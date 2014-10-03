#include <cstring>
#include <quan/three_d/vect.hpp>
#include <quan/stm32/detail/flash.hpp>
#include "flash.hpp"
#include "conv_funcs.hpp"
#include "flash_type_tags.hpp"
#include "../serial_ports.hpp"

namespace {
struct app_symtab_t : quan::stm32::flash::symbol_table {
   app_symtab_t() {}
   ~app_symtab_t() {}
   uint16_t get_symbol_storage_size (uint16_t symidx) const;
   uint16_t get_symtable_size() const;
};
}

// unique id for each type in the structure
// used for array lookup so must start at 0 and be contiguous
// CharAr8 is an array different to a string8 which must be zero terminated
// but types are user defined
//###########################APP LEVEL ###############################
// app level enums for each type in the Flash

namespace {
// app level structure representing the data to be read to or written from cold store
struct {
   quan::three_d::vect<float> mag_offsets;
   bool use_compass;
} flash_image;

// Order is same as flash_type_tags enum above
constexpr flash_symtab::pfn_cstring_to_rep type_cstring_to_rep[] =
{
   cstring_to_rep_Vect3F,
   cstring_to_rep_Bool
};

bool cstring_to_rep_mag_offsets (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
{
   // parse these and see if in range
   return type_cstring_to_rep[ Vect3F] (dest,src);
}

bool cstring_to_rep_use_compass (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
{
   return type_cstring_to_rep[Bool] (dest,src);
}

#define EE_SYMTAB_ENTRY(Val,Info, Readonly) { \
         #Val, \
         get_flash_type_tag<decltype(flash_image.Val)>::value ,\
         & cstring_to_rep_ ## Val, \
         Info ,\
         Readonly \
       }
#if 0
#define EE_SYMTAB_STRING_N(Val,N, info){ \
         #Val, \
         get_flash_type_tag<decltype(flash_image.Val)>::value ,\
         & cstring_to_rep_string<N>, \
         info ,\
         false \
       }
 #endif
struct flash_symtab_entry_t {
   const char* const name;
   uint32_t const type_tag;
   bool (*pfn_cstring_to_rep) (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
   const char * const info;
   bool readonly;
};
 
flash_symtab_entry_t constexpr names[] = {
   EE_SYMTAB_ENTRY(mag_offsets,"[float,float,float] where min = -1000, max =1000",false),
   EE_SYMTAB_ENTRY(use_compass,"true to use compass to set tracker azimuth, else false", false)
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
   sizeof (quan::three_d::vect<float>), // Vect3F
   sizeof (bool)  // Bool
};
 
constexpr flash_symtab::pfn_rep_to_cstring type_rep_to_cstring[] =
{
   rep_to_cstring_Vect3F ,
   rep_to_cstring_Bool
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
   if (! result1) {
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
 
int32_t flash_symtab::get_index (const char* symbol_name)
{
   return get_symtable_index (symbol_name);
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
 
uint8_t flash_check_page (uint8_t n)
{
   uint8_t sum = 0x00;
   
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
   return sum ;
}
 
uint8_t flash_check()
{
   return flash_check_page (1) | flash_check_page (2);
}

 bool default_init_values_to_flash()
  {
      return true;
  }
   

} //namespace
 
bool flash_symtab::init()
{
   if (flash_check() == 0x00) {
   
      user_message ("unwritten flash detected... initialising\n");
     
      if (! quan::stm32::flash::detail::erase (1)) {
         user_message ("init flash page1 failed\n");
         return false ;
      }
      if (! quan::stm32::flash::detail::erase (2)) {
         user_message ("init flash page2 failed\n");
         return false;
      }
       user_message ("\n...flash erased OK\n");
   
   
      // default init of flash values
//     //  user_message ("initing flash values\n");
////      if (! default_init_values_to_flash()){
////         return false;
////      }
   }
   return init_values_from_flash();
}
 