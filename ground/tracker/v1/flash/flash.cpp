/*
 Copyright (c) 2013 -2015 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include <cstring>
#include <quan/three_d/vect.hpp>
#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_convert.hpp>
#include <quan/stm32/detail/flash.hpp>
#include <quan/stm32/flash/flash_error.hpp>
#include "../serial_ports.hpp"
#include "flash.hpp"
//#include "flash_type_tags.hpp"

namespace {

   enum flash_type_tags { Vect3F, Bool};

   // app level create unique integer tags for each type in the flash
   // accessible using these templates
   template <typename T> struct type_to_id;
   template <uint32_t id> struct id_to_type;

   template <> struct type_to_id<quan::three_d::vect<float> > {
      static constexpr uint32_t value = flash_type_tags::Vect3F;
   };
   template <> struct id_to_type<flash_type_tags::Vect3F>{
      typedef quan::three_d::vect<float> type;
   };
   //---------
   template <> struct type_to_id<bool> {
      static constexpr uint32_t value = flash_type_tags::Bool;
   };
   template <> struct id_to_type<flash_type_tags::Bool>{
      typedef bool type;
   };

   // The symbol table  for this app
   struct app_symtab_t : quan::stm32::flash::symbol_table {
      app_symtab_t() {}
      ~app_symtab_t() {}
      uint16_t get_symbol_storage_size (uint16_t symidx) const;
      uint16_t get_symtable_size() const;
   };
//################### 
   // A Representation of the data to be read to or written from Flash
   // as types
   struct flash_variable_type {
      typedef quan::three_d::vect<float> mag_offsets;
      typedef bool use_compass;
   } ;

   // Array of function pointers 
   // for converting a type as text to a bytestream
   // indexed by id in the flash_type_tags enum
   // The order and number 
   // must be the same as the flash_type_tags enum above
   // The use of the id_to_type index hopefully helps!
   constexpr flash_symtab::pfn_text_to_bytestream text_to_bytestream[] =
   {
      &quan::stm32::flash::flash_convert<id_to_type<0>::type>::text_to_bytestream,
      &quan::stm32::flash::flash_convert<id_to_type<1>::type>::text_to_bytestream
   };

   // Array describing the size of a type in flash
   // indexed by id in the flash_type_tags enum
   // the order and number must be the same as the flash_type_tags enum
   // above.
   // There must be a separate entry for each different flash type
   constexpr uint32_t type_tag_to_size[] =
   {
      sizeof (id_to_type<0>::type), 
      sizeof (id_to_type<1>::type) 
   };
    
   // array of function pointers to convert a bytestream to text.
   // indexed by id in the flash_type_tags enum
   // the order and number must be the same as the flash_type_tags enum
   // Can be by type, not by object, since
   // it is assumed that only valid object values have been stored in Flash.
   constexpr flash_symtab::pfn_bytestream_to_text bytestream_to_text[] =
   {
      &quan::stm32::flash::flash_convert<id_to_type<0>::type>::bytestream_to_text,
      &quan::stm32::flash::flash_convert<id_to_type<1>::type>::bytestream_to_text
   };

//####### The object symtable itself ###########################

   // Use the macro to make flash_symtab_entry_t entries for the names array below
   // The Name is the string name representing the variable
   // the Checkfun is a function which takes a void* arg
   // intended to check the value is in range
   #define EE_SYMTAB_ENTRY(Name, CheckFun,Info, Readonly) { \
            #Name, \
            type_to_id<flash_variable_type::Name >::value ,\
            CheckFun, \
            Info ,\
            Readonly \
          }

   // A symbol table entry class
   // name of the symbol
   // with tag representing the type
   // and some info
   // and a range check function. (The void* is changed to a pointer to the type
   // to be checked
   struct flash_symtab_entry_t {
      const char* const name;
      uint32_t const type_tag;
      bool (*pfn_validity_check)(void*);
      const char * const info;
      bool readonly;
   };
    
// mag offsets range check function
// the void* arg is converted to a pointer to the type to be checked
// For out of range values an error message should be generated
// using user_eror function
   bool mag_offsets_check(void* p)
   {
        flash_variable_type::mag_offsets * pv =  (flash_variable_type::mag_offsets*) p;
        if ( pv == nullptr){
          return false;
        }
        bool value_good = (pv->x < 1000.f) && ( pv->x > -1000.f)
                  &&  (pv->y < 1000.f) &&  (pv->y > -1000.f)
                  &&  (pv->z < 1000.f)  && (pv->z > -1000.f);
         if ( value_good){
            return true;
         }else{
            user_error("mag_offsets out of range");
            return false;
         }
   }

  // use this check funtion if there is no error checking required
   bool nop_check (void* p) { return true;}

   flash_symtab_entry_t constexpr names[] = {
      EE_SYMTAB_ENTRY(mag_offsets,mag_offsets_check,"[float,float,float] where min=-1000, max=1000",false),
      EE_SYMTAB_ENTRY(use_compass,nop_check,"true = use compass to set tracker azimuth, else manual", false)
   };
    
   // very slow lookup of a name in the symtab
   // to return a symbol index
   // or -1 if not found
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

   uint16_t get_type_index (uint16_t symidx)
   {
      return names[symidx].type_tag;
   }
   
   // get size of a type by index
    
   uint16_t get_type_size (uint16_t typeidx)
   {
      return type_tag_to_size[typeidx];
   }
    
   app_symtab_t app_symtab;

   /*
      get the check function for a symbol
   */
   bool (* get_check_fun(uint16_t symindex)) (void*)
   {
      if (symindex < app_symtab.get_symtable_size()) {
         return names[symindex].pfn_validity_check;
      } else {
         return nullptr;
      }
   }
 
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
   return sizeof (names)/sizeof (flash_symtab_entry_t);
}
 
uint16_t flash_symtab::get_num_elements()
{
   return app_symtab.get_symtable_size();
}
 
bool flash_symtab::write_from_bytestream (uint16_t symidx, quan::dynarray<uint8_t> const & symbol_value)
{
   return quan::stm32::flash::write_symbol (app_symtab,symidx,symbol_value);
}
 
bool flash_symtab::read (uint16_t symidx, quan::dynarray<uint8_t> & symbol_value)
{
   return quan::stm32::flash::read_symbol (app_symtab,symidx,symbol_value);
}

bool flash_symtab::write_from_text (uint16_t symbol_index,quan::dynarray<char> const & value)
{
   uint16_t const symbol_size = flash_symtab::get_size (symbol_index);
   // add check not 0 or neg
   quan::dynarray<uint8_t> bytestream { (size_t) symbol_size,main_alloc_failed};
   if (!bytestream.good()) {
      return false;
   }
   auto const text_to_bytestream_fun = get_text_to_bytestream_fun (symbol_index);
   auto const check_fun = get_check_fun(symbol_index);
   bool result1 = text_to_bytestream_fun (bytestream,value,check_fun);
   if (! result1) {
      return false;
   }
   return flash_symtab::write_from_bytestream (symbol_index,bytestream);
}
 
bool flash_symtab::read_to_text (
   uint16_t symbol_index, quan::dynarray<char> & value)
{
   uint16_t const symbol_size = flash_symtab::get_size (symbol_index);
   quan::dynarray<uint8_t> bytestream {symbol_size,main_alloc_failed};
   if (!bytestream.good()) {
      return false;
   }
   if (!flash_symtab::read (symbol_index, bytestream)) {
      return false;
   }
   auto const rep_to_string_fun = get_bytestream_to_text_fun (symbol_index);
   return rep_to_string_fun (value,bytestream) ;
}
 
bool flash_symtab::exists (uint16_t symindex)
{
   if (symindex < app_symtab.get_symtable_size()) {
      return quan::stm32::flash::have_symbol (app_symtab,symindex);
   } else {
      return false;
   }
}
bool flash_symtab::is_defined(const char* symbol_name)
{
   int32_t idx =  flash_symtab::get_index(symbol_name);
   return  (idx != -1) && flash_symtab::exists(idx);
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
flash_symtab::pfn_text_to_bytestream flash_symtab::get_text_to_bytestream_fun (uint16_t symidx)
{
   //return names[symidx].pfn_text_to_bytestream;
   return text_to_bytestream[get_type_index (symidx) ];
}

// coming out data assumed ok.
flash_symtab::pfn_bytestream_to_text flash_symtab::get_bytestream_to_text_fun (uint16_t symidx)
{
   return bytestream_to_text[get_type_index (symidx) ];
}
 
namespace {
 
/*
See if flash page has been written.
 Currently (with Linux version of ST-Link), the default 
 after uploading is to clear verything to 0
  so starting from 0 or each byte with the sum.
  if reult is 0 then page is empty
*/
uint8_t flash_check_page (uint8_t n)
{
   uint8_t sum = 0x00;
   
   volatile uint8_t * pn 
      = (volatile uint8_t*)quan::stm32::flash::detail::get_page_address (n);
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
   return flash_check_page(1) | flash_check_page(2);
}

} //namespace
 
bool init_values_from_flash();
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
   }
   return init_values_from_flash();
}
 