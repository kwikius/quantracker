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
#include <quan/stm32/flash/flash_convert/vect3f.hpp>
#include <quan/stm32/flash/flash_convert/bool.hpp>
#include <quan/stm32/detail/flash.hpp>
#include <quan/stm32/flash/flash_error.hpp>
#include "../serial_ports.hpp"
#include "flash.hpp"

namespace {

   // The enum holds numeric ids , one for each different type in the flash vars 
   enum flash_type_tags { Vect3F =0, Bool =1};
   // use these templates to map types to ids and vice versa
   template <typename T> struct type_to_id;
   template <uint32_t id> struct id_to_type;

   //--------- for Vect3F
   template <> struct type_to_id<quan::three_d::vect<float> > {
      static constexpr uint32_t value = flash_type_tags::Vect3F;
   };
   template <> struct id_to_type<flash_type_tags::Vect3F>{
      typedef quan::three_d::vect<float> type;
   };
   //--------- for Bool
   template <> struct type_to_id<bool> {
      static constexpr uint32_t value = flash_type_tags::Bool;
   };
   template <> struct id_to_type<flash_type_tags::Bool>{
      typedef bool type;
   };

}

namespace quan{ namespace stm32{ namespace flash{

      // per app ids for types. Since types may be arranged different ways this is 
      // for the app to define as here
      uint32_t quan::stm32::flash::get_flash_typeid_impl<bool>::apply() 
         {return type_to_id<bool>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<quan::three_d::vect<float> >::apply() 
         {return type_to_id<quan::three_d::vect<float>>::value;}
}}}

namespace {

   // The symbol table for this app. Provides the implementation of the 
   // various data required by the symbol table
   struct app_symtab_t : quan::stm32::flash::symbol_table {
      app_symtab_t() {}
      ~app_symtab_t() {}
      uint16_t get_symbol_storage_size (uint16_t symbol_index) const;
      uint16_t get_symtable_size() const;
      int32_t get_index( const char* const symbol_name) const;
      const char* get_name (uint16_t symbol_index)const;
      const char* get_info (uint16_t symbol_index) const;
      bool get_readonly_status(uint16_t symbol_index,bool & result)const;
      pfn_text_to_bytestream get_text_to_bytestream_fun( uint16_t symbol_index) const;
      pfn_bytestream_to_text get_bytestream_to_text_fun( uint16_t symbol_index) const;
      bool get_typeid(uint16_t symbol_index, uint32_t & dest) const;
      bool init() const;
      pfn_check_function get_check_function(uint16_t symbol_index)const;
   };
//################### 
   // A Representation of the data to be read to or written from Flash
   // as types
   struct flash_variable_type {
      typedef quan::three_d::vect<float> mag_offsets;
      typedef bool                       use_compass;
   } ;

   // Array of function pointers 
   // for converting a type as text to a bytestream
   // indexed by id in the flash_type_tags enum
   // The order and number 
   // must be the same as the flash_type_tags enum above
   // The use of the id_to_type index hopefully helps!
   constexpr quan::stm32::flash::symbol_table::pfn_text_to_bytestream text_to_bytestream[] =
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
    
   // array of function pointers to convert a bytestream to text (user represntation)
   // indexed by id in the flash_type_tags enum
   // the order and number must be the same as the flash_type_tags enum
   // Can be by type, not by object, since
   // it is assumed that only valid object values have been stored in Flash.
   constexpr quan::stm32::flash::symbol_table::pfn_bytestream_to_text bytestream_to_text[] =
   {
      &quan::stm32::flash::flash_convert<id_to_type<0>::type>::bytestream_to_text,
      &quan::stm32::flash::flash_convert<id_to_type<1>::type>::bytestream_to_text
   };

   // Use the macro to make flash_symtab_entry_t entries for the names symbol table below
   // The Name is the string name representing the variable without dquotes
   // the Checkfun is a function which takes a void* arg
   // intended to check the value is in range
   #define EE_SYMTAB_ENTRY(Name, CheckFun,Info, Readonly) { \
            #Name, \
            type_to_id<flash_variable_type::Name >::value ,\
            CheckFun, \
            Info ,\
            Readonly \
          }

   // The symbol table entry class.
   // name of the symbol
   // with tag representing the type according to the flash_type_tags enum above
   // some user help info
   // and a range check function. 
   // (The void* is actually a pointer to the value to be checked)
   struct flash_symtab_entry_t {
      const char* const name;
      uint32_t const type_tag;
      bool (*pfn_validity_check)(void*);
      const char * const info;
      bool readonly;
   };
    
// Range check functions for validating user input values to update flash variable values
// the void* arg is converted to a pointer to the type to be checked 
// For out of range values an error message should be generated
// which is output to the user to help diagnose what is wrong
// using user_error(str) function
  
// use this check funtion if there is no error checking required e.g for bool
   bool nop_check (void* p) { return true;}

 // Example. The function that checks the "mag_offsets" variable is in limits
   bool mag_offsets_check(void* p)
   {
      if ( p == nullptr){
         return false;
      }
      // convert the void * to a pointer in the type of the value to be range checked
      flash_variable_type::mag_offsets * pv = (flash_variable_type::mag_offsets*) p;
      
      bool const value_good = (pv->x < 1000.f) && ( pv->x > -1000.f)
      &&  (pv->y < 1000.f) &&  (pv->y > -1000.f)
      &&  (pv->z < 1000.f)  && (pv->z > -1000.f);
      if ( value_good){
         return true;
      }else{
         user_error("mag_offsets range: -999 to 999");
         return false;
      }
   }

//####### The object symtable itself ###########################
   // Add  One element per Flash variable
   flash_symtab_entry_t constexpr names[] = {
      EE_SYMTAB_ENTRY(mag_offsets,mag_offsets_check,"[float,float,float] range: -999 to 999",false),
      EE_SYMTAB_ENTRY(use_compass,nop_check,"true = use compass to set tracker azimuth", false)
   };
    
   #undef EE_SYMTAB_ENTRY

   // get  
   uint16_t get_type_index (uint16_t symbol_index)
   {
      return names[symbol_index].type_tag;
   }
   
   // get size of a type by index
   uint16_t get_type_size (uint16_t typeidx)
   {
      return type_tag_to_size[typeidx];
   }
    
   app_symtab_t app_symtab;

} // namespace

bool app_symtab_t::get_typeid(uint16_t symbol_index, uint32_t & dest) const
{
    if( symbol_index < this->get_symtable_size()){
       dest =  names[symbol_index].type_tag;
       return true;
    }else{
      return false;
    }
}

app_symtab_t::pfn_check_function app_symtab_t::get_check_function(uint16_t symbol_index)const
{
   if (symbol_index < this->get_symtable_size()) {
         return names[symbol_index].pfn_validity_check;
   } else {
      return nullptr;
   }
}

// implement global function to access the symboltable
quan::stm32::flash::symbol_table const & quan::stm32::flash::get_app_symbol_table()
{
   return app_symtab;
}

const char* app_symtab_t::get_info (uint16_t symbol_index)const
{
   if (symbol_index < this->get_symtable_size()) {
      return names[symbol_index].info;
   } else {
      return nullptr;
   }
}

bool app_symtab_t::get_readonly_status (uint16_t symbol_index,bool & result)const
{
   if (symbol_index < this->get_symtable_size()) {
      result = names[symbol_index].readonly;
      return true;
   } else {
      return false;
   }
}

uint16_t app_symtab_t::get_symbol_storage_size (uint16_t symbol_index) const
{
    if (symbol_index < this->get_symtable_size()) {
      return get_type_size (get_type_index (symbol_index));
    }else{
      return 0U;
    }
}

uint16_t app_symtab_t::get_symtable_size() const
{
   return sizeof (names)/sizeof (flash_symtab_entry_t);
}

int32_t app_symtab_t::get_index (const char* symbol_name)const
{
      int count = 0;
      for (auto entry : names) {
         if (strcmp (entry.name, symbol_name) == 0) {
            return count;
         } else {
            ++count;
         }
      }
      return -1; // not found
}

const char* app_symtab_t::get_name (uint16_t symbol_index)const
{
   if (symbol_index < this->get_symtable_size()) {
      return names[symbol_index].name;
   } else {
      return nullptr;
   }
}

app_symtab_t::pfn_text_to_bytestream app_symtab_t::get_text_to_bytestream_fun (uint16_t symbol_index)const
{
   return text_to_bytestream[get_type_index (symbol_index) ];
}

// coming out data assumed ok.
app_symtab_t::pfn_bytestream_to_text app_symtab_t::get_bytestream_to_text_fun (uint16_t symbol_index)const
{
   return bytestream_to_text[get_type_index (symbol_index) ];
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
bool app_symtab_t::init()const 
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
 