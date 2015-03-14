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
#include <quan/stm32/detail/flash.hpp>
#include "../serial_ports.hpp"
#include "flash.hpp"
#include "flash_error.hpp"
#include "flash_convert.hpp"
#include "flash_type_tags.hpp"

namespace {
   // The symtab for this app
   struct app_symtab_t : quan::stm32::flash::symbol_table {
      app_symtab_t() {}
      ~app_symtab_t() {}
      uint16_t get_symbol_storage_size (uint16_t symidx) const;
      uint16_t get_symtable_size() const;
   };

   // Representation of the data to be read to or written from Flash
   // as a structure for this app
   struct {
      quan::three_d::vect<float> mag_offsets;
      bool use_compass;
   } flash_image;

   // array of functions for converting text to a bytestream
   // Each function in the array converts from text to the type
   // at the ame index in the flash_type_tags enum
   // The order must be the same as the flash_type_tags enum in flash_type_tags.hpp
   constexpr flash_symtab::pfn_text_to_bytestream text_to_bytestream[] =
   {
      &flash_convert<id_to_type<0>::type>::text_to_bytestream,
      &flash_convert<id_to_type<1>::type>::text_to_bytestream
   };
   // in converting to bytestream may need to convert then check values
// otherwise can just use type conv func as here
   bool text_to_bytestream_mag_offsets (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
   {
      return text_to_bytestream[ type_to_id<decltype(flash_image.mag_offsets)>::value] (dest,src);
   }

   bool text_to_bytestream_use_compass (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
   {
      return text_to_bytestream[type_to_id<decltype(flash_image.use_compass)>::value] (dest,src);
   }

   // Array describing the size of a type in flash
   // the order must be the same as the flash_type_tags enum
   // in flash_type_tags.hpp
   // There must be a separate entry for each different flash typ
   constexpr uint32_t type_tag_to_size[] =
   {
      sizeof (id_to_type<0>::type), 
      sizeof (id_to_type<1>::type) 
   };
    
   /* array of fun_ptrs to convert the bytestream to text*/
   constexpr flash_symtab::pfn_bytestream_to_text bytestream_to_text[] =
   {
      &flash_convert<id_to_type<0>::type>::bytestream_to_text,
      &flash_convert<id_to_type<1>::type>::bytestream_to_text
   };

   #define EE_SYMTAB_ENTRY(Val,Info, Readonly) { \
            #Val, \
            type_to_id<decltype(flash_image.Val)>::value ,\
            & text_to_bytestream_ ## Val, \
            Info ,\
            Readonly \
          }

   // name of the symbol
   // with tag representing the type
   // and some info
   struct flash_symtab_entry_t {
      const char* const name;
      uint32_t const type_tag;
      bool (*pfn_text_to_bytestream) (quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
      const char * const info;
      bool readonly;
   };
    
   flash_symtab_entry_t constexpr names[] = {
      EE_SYMTAB_ENTRY(mag_offsets,"[float,float,float] where min = -1000, max =1000",false),
      EE_SYMTAB_ENTRY(use_compass,"true to use compass to set tracker azimuth, else false", false)
   };
    
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
   
   bool result1 = text_to_bytestream_fun (bytestream,value);
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
   return names[symidx].pfn_text_to_bytestream;
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
 