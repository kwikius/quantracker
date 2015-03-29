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
#include <quan/user.hpp>
#include <quan/stm32/flash/flash_error.hpp>

namespace {

   // The symbol table for this app. Provides the implementation of the 
   // various virtual functions data required by the symbol table
   struct app_symtab_t : quan::stm32::flash::symbol_table {
      app_symtab_t() {}
      ~app_symtab_t() {}
      uint16_t get_symbol_storage_size (int32_t symbol_index) const;
      uint16_t get_symtable_size() const;
      int32_t get_index( const char* const symbol_name) const;
      const char* get_name (int32_t symbol_index)const;
      const char* get_info (int32_t symbol_index) const;
      bool get_readonly_status(int32_t symbol_index,bool & result)const;
      bool get_typeid(int32_t symbol_index, uint32_t & dest) const;
      bool init() const;
      pfn_check_function get_check_function(int32_t symbol_index)const;
   };

// ####################### access to types via the lookup name ####################
   // Not required for your implementation
   // This structure is just used to map the type of each flash variable to its name
   // for use by the EE_SYMTAB_ENTRY macro below. The order isnt important
   // except for neatness !
   // no reference/const/volatile/pointers/T(*)(...) here please, just unadorned value_types!
   struct flash_variable_type {
      typedef bool                           show_home;
      typedef quan::three_d::vect<int32_t>   osd_home_pos;
      typedef bool                           show_compass;
      typedef quan::three_d::vect<int32_t>   osd_compass_pos;
      typedef bool                           show_altitude;
      typedef quan::three_d::vect<int32_t>   osd_altitude_pos;
      typedef bool                           show_gps_no_fix;
      typedef quan::three_d::vect<int32_t>   osd_gps_no_fix_pos;
      typedef bool                           show_afcl_horizon;
      typedef int16_t                        osd_afcl_horizon_pitch_adj;
   } ;
   //#################### Per object range checking ########################
    
// Range check callback functions are used per object rather than per type
// for validating user input values before they are used to update flash variable values
// the void* arg is converted to a pointer to the type to be checked.
// For out of range values an error message should be generated
// which is output to the user to help diagnose what is wrong
// using user_error(str) function
 // examples follow...
 //----------------
// no-op range checking
// This check function can be used if there is no validity checking required e.g for bool variables
   constexpr bool nop_check (void* p) { return true;}
//-----------------

   // check that display pos vars are in range
   bool display_pos_check(void* p)
   {
      if ( p == nullptr){
         return false;
      }
      // convert the void * to a pointer in the type of the value to be range checked
      // type of home_pos used, assumes all same type
      flash_variable_type::osd_home_pos * pv = (flash_variable_type::osd_home_pos*) p;
      bool const value_good = (pv->x < 500) && ( pv->x > -500)
      &&  (pv->y < 500) &&  (pv->y > -500)
      &&  (pv->z < 500)  && (pv->z > -500);
      if ( value_good){
         return true;
      }else{
         quan::user_error("display pos range: -499 to 499");
         return false;
      }
   }
   bool afcl_horizon_pitch_adj_check(void* p)
   {
      if ( p == nullptr){
         return false;
      }
      flash_variable_type::osd_afcl_horizon_pitch_adj * pv = (flash_variable_type::osd_afcl_horizon_pitch_adj*) p;
      if ( (*pv < 21) &&  (*pv > -21)){
         return true;
      }else{
         quan::user_error("afcl horizon range: -20 to 20 deg");
         return false;
      }
   }

   // The symbol table entry structure for a flash variable
   struct flash_symtab_entry_t {
      const char* const name;  // user name of the symbol
      uint32_t const type_tag;  // id representing the type of the variable
      bool (*pfn_validity_check)(void*); //range check function 
      const char * const info; // some help about the flash variable
      bool readonly;  // if the value is readonly ( not writable from user flash menu)
   };

   // Use the macro to add flash_symtab_entry_t entries for the names symbol table below
   #define EE_SYMTAB_ENTRY(Name, CheckFun,Info, Readonly) { \
            #Name, \
            quan::stm32::flash::type_to_flash_id<flash_variable_type::Name >::value ,\
            CheckFun, \
            Info ,\
            Readonly \
          }

//####### The object symtable itself ###########################
 
   flash_symtab_entry_t constexpr names[] = {
       EE_SYMTAB_ENTRY(show_home, nop_check,"true/false to show home distance",false)
      ,EE_SYMTAB_ENTRY(osd_home_pos,display_pos_check,"[int x, int y_pal, int y_ntsc] range: -499 to 499",false)
      ,EE_SYMTAB_ENTRY(show_compass, nop_check,"true/false to show compass",false)
      ,EE_SYMTAB_ENTRY(osd_compass_pos,display_pos_check,"[int x, int y_pal, int y_ntsc] range: -499 to 499",false)
      ,EE_SYMTAB_ENTRY(show_altitude, nop_check,"true/false to show home alt",false)
      ,EE_SYMTAB_ENTRY(osd_altitude_pos,display_pos_check,"[int x, int y_pal, int y_ntsc] range: -499 to 499",false)
      ,EE_SYMTAB_ENTRY(show_gps_no_fix, nop_check,"true/false to show no fix gps pos",false)
      ,EE_SYMTAB_ENTRY(osd_gps_no_fix_pos,display_pos_check,"[int x, int y_pal, int y_ntsc] range: -499 to 499",false)
      ,EE_SYMTAB_ENTRY(show_afcl_horizon, nop_check,"true/false to show no fix gps pos",false)
      ,EE_SYMTAB_ENTRY(osd_afcl_horizon_pitch_adj,afcl_horizon_pitch_adj_check,"range: -20 to 20 (degrees)",false)

   };
    // ok we are done with this !
   #undef EE_SYMTAB_ENTRY
 
   // requires valid symbol_index -- not checked
//   constexpr uint16_t get_type_index (int32_t symbol_index)
//   {
//      return names[static_cast<uint16_t>(symbol_index)].type_tag;
//   }
   
   uint16_t get_type_size (uint16_t typeidx)
   {
      return app_symtab_t::type_tag_to_size[typeidx];
   }
  
    
   //instantiate the application symbol table
   app_symtab_t app_symtab;

    // get size of a type by index
   // requires valid type_index -- not checked


} // namespace


bool app_symtab_t::get_typeid(int32_t symbol_index, uint32_t & dest) const
{
    if( this->is_valid_symbol_index(symbol_index)){
       dest = names[static_cast<uint16_t>(symbol_index)].type_tag;
       return true;
    }else{
      return false;
    }
}
 // requires valid symbol_index -- not checked
app_symtab_t::pfn_check_function app_symtab_t::get_check_function(int32_t symbol_index)const
{
   return names[static_cast<uint16_t>(symbol_index)].pfn_validity_check;
}

// implement global function to access the symboltable
quan::stm32::flash::symbol_table const & quan::stm32::flash::get_app_symbol_table()
{
   return app_symtab;
}

const char* app_symtab_t::get_info (int32_t symbol_index)const
{
   if (this->is_valid_symbol_index(symbol_index)) {
      return names[static_cast<uint16_t>(symbol_index)].info;
   } else {
      return nullptr;
   }
}

bool app_symtab_t::get_readonly_status (int32_t symbol_index,bool & result)const
{
   if (this->is_valid_symbol_index(symbol_index)) {
      result = names[static_cast<uint16_t>(symbol_index)].readonly;
      return true;
   } else {
      return false;
   }
}

uint16_t app_symtab_t::get_symbol_storage_size (int32_t symbol_index) const
{
    if (this->is_valid_symbol_index(symbol_index)) {
      return get_type_size (names[static_cast<uint16_t>(symbol_index)].type_tag);
      //return get_type_size (get_type_index (symbol_index));
    }else{
      return 0U;
    }
}

uint16_t app_symtab_t::get_symtable_size() const
{
   return sizeof (names)/sizeof (flash_symtab_entry_t);
}

/*
 slow but who cares really ?
*/
int32_t app_symtab_t::get_index (const char* symbol_name)const
{
      int32_t count = 0;
      for (auto entry : names) {
         if (strcmp (entry.name, symbol_name) == 0) {
            return count;
         } else {
            ++count;
         }
      }
      return -1; // not found
}

const char* app_symtab_t::get_name (int32_t symbol_index)const
{
   if (this->is_valid_symbol_index(symbol_index)) {
      return names[symbol_index].name;
   } else {
      return nullptr;
   }
}

// defined in symbology.cpp
bool init_values_from_flash();
bool app_symtab_t::init()const 
{
   if (!flash_init()){
      return false;
   }
   return init_values_from_flash();
}

// if returns false
// then something bad happened
// then try the report_errors function once you have a user i/o (e.g in mavlink_task startup
// or maybe should create a fail task
bool initialise_flash()
{
  return app_symtab.init();
}
 