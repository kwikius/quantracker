#include <cstring>
#include <quan/three_d/vect.hpp>
#include <quan/stm32/detail/flash.hpp>
#include "../serial_ports.hpp"
#include "flash.hpp"
#include "conv_funcs.hpp"

namespace{
   struct app_symtab_t : quan::stm32::flash::symbol_table{
        app_symtab_t(){}
        ~app_symtab_t(){}
        uint16_t get_symbol_storage_size(uint16_t symidx)const;
        uint16_t get_symtable_size()const;
   };
}

void user_message (const char* str);
// unique id for each type in the structure
// used for array lookup so must start at 0 and be contiguous
// CharAr8 is an array different to a string8 which must be zero terminated
// but types are user defined
//###########################APP LEVEL ###############################
// app level enums for each type in the Flash
namespace {

   enum flash_type_tags { Char, MagVect3F, CharAr8};

   // app level create unique integer tags for each type in the eeprom structure
   // accessible using these templates
   template <typename T> struct get_flash_type_tag;

   template <> struct get_flash_type_tag<char> {
      static constexpr uint32_t value = flash_type_tags::Char;
   };
   template <> struct get_flash_type_tag<quan::three_d::vect<float> > {
      static constexpr uint32_t value = flash_type_tags::MagVect3F;
   };
   template <> struct get_flash_type_tag<char[8]> {
      static constexpr uint32_t value = flash_type_tags::CharAr8;
   };

   // app level structure representing the data to be read to or written from cold store
   struct {
      char magic[8];
      char cx;
      quan::three_d::vect<float> mag_offsets;
      char string [8];
   } flash_image;

   constexpr flash_symtab::pfn_cstring_to_rep type_cstring_to_rep[] =
   {
      cstring_to_rep_Char,
      cstring_to_rep_MagVect3F,
      cstring_to_rep_CharAr8
   };

   // app level functions for verifying a new value for the symbol is ok
   bool cstring_to_rep_magic (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
   {
      // magic should be a zero terminated string > 4 <= 8 chars inc null at end
      // read into local
      //  (alpha| '_')+(alpha |digit)*
      auto src_len = src.get_num_elements();
      if (src_len > 8) {
         user_error ("string too long");
         return false;
      } else {
         if (src_len < 2) { // min 1 char and 0
            user_error ("expected string");
            return false;
         }
         
         quan::dynarray<uint8_t> checked {8,main_alloc_failed};
         if (! checked.good()) {
            return false;
         }
         
         for (uint8_t i = 0; i < 8; ++i) {
            if (i < (src_len-1)) {
               char ch = src.get() [i];
               if ( ( (i ==0) && (isalpha (ch) || ch == '_')) || ( (i !=0) && isalnum (ch))) {
                  checked.get() [i] = (uint8_t) ch;
               } else {
                  user_error ("invalid value");
                  return false;
               }
            } else {
               if ( (i == (src_len-1)) && (src.get() [i] != '\0')) {
                  quan::error (fn_cstring_to_rep_magic, quan::detail::unterminated_string_constant);
                  return false;
               } else {
                  checked.get() [i] = (uint8_t) '\0';
               }
            }
         }
         if (dest.realloc (8)) {
            memcpy (dest.get(),src.get(),8);
            return true;
         } else {
            quan::error (fn_cstring_to_rep_magic, quan::detail::out_of_heap_memory);
            return false;
         }
      }
   }
    
   bool cstring_to_rep_cx (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
   {
      if (strlen (src.get()) !=1) {
         user_error ("should be length 1");
         return false;
      } else {
         if (! dest.realloc (1)) {
            quan::error (fn_cstring_to_rep_Char, quan::detail::out_of_heap_memory);
            return false;
         }
         else {
            (dest.get()) [0] = (uint8_t)(src.get() [0]);
            return true;
         }
      }
   }
   bool cstring_to_rep_mag_offsets (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
   {
      // parse these and see if in range
      return type_cstring_to_rep[ MagVect3F] (dest,src);
   }
   // string of useable size N ( add a zero terminator)
   // maybe should be length inc terminator?
   template <uint32_t N>
   bool cstring_to_rep_string (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
   {
      auto src_len = src.get_num_elements();
      if (src_len > N) {
         user_error ("string too long");
         return false;
      } else {
         if (! dest.realloc (N)) {
            quan::error (fn_cstring_to_rep_String, quan::detail::out_of_heap_memory);
            return false;
         }
         for (auto i = 0U; i < (N); ++i) {
            dest.get() [i] =  static_cast<uint8_t>(0);
         }
         for (auto i = 0U; i < src_len; ++i) {
            dest.get() [i] = src.get() [i];
         }
         return true;
      }
   }
    
   #define EE_SYMTAB_ENTRY(Val,info) { \
         #Val, \
         get_flash_type_tag<decltype(flash_image.Val)>::value ,\
         & cstring_to_rep_ ## Val, \
         info ,\
         false \
       }
   #define EE_SYMTAB_STRING_N(Val,N, info){ \
         #Val, \
         get_flash_type_tag<decltype(flash_image.Val)>::value ,\
         & cstring_to_rep_string<N>, \
         info ,\
         false \
       }
    
   struct flash_symtab_entry_t {
      const char* const name;
      uint32_t const type_tag;
      bool (*pfn_cstring_to_rep) (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
      const char * const info;
      bool readonly;
   };
    
   flash_symtab_entry_t constexpr names[] = {
      // name , descr
      EE_SYMTAB_ENTRY (magic,"char[8] where (alpha|\'_\')+(alpha |digit)*)\'0\'"),
      EE_SYMTAB_ENTRY (cx,"char"),
      EE_SYMTAB_ENTRY (mag_offsets,"[float,float,float] where min = -1000, max =1000"),
      EE_SYMTAB_STRING_N (string,8,"char[8] where (alpha)+\'0\'")
   };
    
   constexpr uint32_t names_size = sizeof (names) /sizeof (flash_symtab_entry_t);
    
   int32_t get_symtable_index (const char* symbol)
   {
      int count = 0;
      for (auto entry : names) {
         if (strcmp (entry.name, symbol) == 0) {
            return count;
         } else {
            ++count;
         }
      }
      return -1; // not found
   }
    
   #undef EE_SYMTAB_ENTRY
   // the order must be the same as the flash_type_tags enum
   constexpr uint32_t type_tag_to_size[] =
   {
      sizeof (char), // Char
      sizeof (quan::three_d::vect<float>), // MagVect3F
      sizeof (char[8]) // CharAr8
   };
    
   constexpr flash_symtab::pfn_rep_to_cstring type_rep_to_cstring[] =
   {
      rep_to_cstring_Char,
      rep_to_cstring_MagVect3F,
      rep_to_cstring_CharAr8
   };
    
   //interface for meeting requirements of archetype
   uint16_t get_type_index (uint16_t symidx)
   {
      return names[symidx].type_tag;
   }
    
   uint16_t get_type_size (uint16_t typeidx)
   {
      return type_tag_to_size[typeidx];
   }

   app_symtab_t app_symtab;

} // namespace
 
 
const char* flash_symtab::get_name (uint16_t symindex)
{
   if (symindex < app_symtab.get_symtable_size()) {
      return names[symindex].name;
   } else {
      return nullptr;
   }
}
 
const char* flash_symtab::get_info (uint16_t symindex)
{
   if (symindex < app_symtab.get_symtable_size()) {
      return names[symindex].info;
   } else {
      return nullptr;
   }
}
 
bool flash_symtab::get_readonly_status (uint16_t symindex,bool & result)
{
   if (symindex < app_symtab.get_symtable_size()) {
      result = names[symindex].readonly;
      return true;
   } else {
      return false;
   }
}
 
uint16_t app_symtab_t::get_symbol_storage_size (uint16_t symidx) const
{
   return get_type_size (get_type_index (symidx));
}
 
uint16_t app_symtab_t::get_symtable_size() const
{
   return names_size;
}
 
uint16_t flash_symtab::get_num_elements()
{
   return app_symtab.get_symtable_size();
}
 
bool flash_symtab::write (uint16_t symidx, quan::dynarray<uint8_t> const & symbol_value)
{
   return quan::stm32::flash::write_symbol (app_symtab,symidx,symbol_value);
}
 
bool flash_symtab::read (uint16_t symidx, quan::dynarray<uint8_t> & symbol_value)
{
   return quan::stm32::flash::read_symbol (app_symtab,symidx,symbol_value);
}
 
bool flash_symtab::write_from_string (uint16_t symbol_index,quan::dynarray<char> const & value)
{
   uint16_t const symbol_size = flash_symtab::get_size (symbol_index);
   // add check not 0 or neg
   quan::dynarray<uint8_t> reparray { (size_t) symbol_size,main_alloc_failed};
   if (!reparray.good()) {
      return false;
   }
   auto const string_to_rep_fun = get_string_to_rep_fun (symbol_index);

   bool result1 = string_to_rep_fun (reparray,value);
   if (! result1){
     return false;
   } 
   return flash_symtab::write (symbol_index,reparray);
}
 
bool flash_symtab::read_to_string (
     uint16_t symbol_index, quan::dynarray<char> & value)
{
   uint16_t const symbol_size = flash_symtab::get_size (symbol_index);
   quan::dynarray<uint8_t> reparray {symbol_size,main_alloc_failed};
   if (!reparray.good()) {
      return false;
   }
   if (!flash_symtab::read (symbol_index, reparray)) {
      return false;
   }
   auto const rep_to_string_fun = get_rep_to_string_fun (symbol_index);
   return rep_to_string_fun (value,reparray) ;
}
 
bool flash_symtab::exists (uint16_t symindex)
{
   if (symindex < app_symtab.get_symtable_size()) {
      return quan::stm32::flash::have_symbol (app_symtab,symindex);
   } else {
      return false;
   }
}
 
uint16_t flash_symtab::get_size (uint16_t symidx)
{
   return app_symtab.get_symbol_storage_size (symidx);
}
 
int32_t flash_symtab::get_index (quan::dynarray<char> const & symbol)
{
   return get_symtable_index (symbol.get());
}
 
// per symbol as string needs to be checked for validity
flash_symtab::pfn_cstring_to_rep flash_symtab::get_string_to_rep_fun (uint16_t symidx)
{
   return names[symidx].pfn_cstring_to_rep;
}
 
// coming out data assumed ok.
flash_symtab::pfn_rep_to_cstring flash_symtab::get_rep_to_string_fun (uint16_t symidx)
{
   return type_rep_to_cstring[get_type_index (symidx) ];
}
 
namespace {

   uint8_t flash_check_page(uint8_t n)
   {
      uint8_t sum = 0;
      
      volatile uint8_t * pn = (volatile uint8_t*)
                              quan::stm32::flash::detail::get_page_address (n);
      uint32_t  pn_size = quan::stm32::flash::detail::get_page_size (n);
      if (pn_size ==0) {
         user_message ("invalid flash page\n");
         return 0;
      }
      for (uint32_t i = 0; i < pn_size; ++i) {
         sum |= *pn;
         ++pn;
      }
      return sum;
   }

   uint8_t flash_check()
   {
      return flash_check_page(1) | flash_check_page(2);
   }

} //namespace

bool flash_symtab::init()
{
   if (flash_check() == 0) {
      user_message ("virgin flash detected... erase Y/N ?\n");
      while (!sliprings::serial_port::in_avail()) {;}
      char ch = sliprings::serial_port::get();
      if ( (ch == 'Y') || (ch == 'y')) {
         while (! sliprings::serial_port::tx_reg_empty()) {;}
        
         if (! quan::stm32::flash::detail::erase (1)) {
            user_message ("erase page1 failed\n");
            return false ;
         }
         if (! quan::stm32::flash::detail::erase (2)) {
            user_message ("erase page2 failed\n");
            return false;
         }
         user_message("\n...flash erased OK\n");
         return true;
      }
   }else{ // nothing to do
      return true;
   }
   // shouldnt get here
   return false;
}
