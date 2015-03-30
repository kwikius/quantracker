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
      uint16_t get_symtable_size() const;
      quan::stm32::flash::symtab_entry_t const * get_symbol_table()const;
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
// for validating user input values before they are used to update flash variable values.
//
// the void* arg is converted to a pointer to the type to be checked.
//
// For out of range values an error message should be generated
// which is output to the user to help diagnose what is wrong
// using user_error(str) function
// examples follow...
//----------------
// no-op range checking
// This check function can be used if there is no validity checking required e.g for bool variables
   constexpr bool nop_check (void* p) { return true;}
//-----------------

   // check that display pos vars are in a sensible range
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

//####### The object symtable itself ###########################
 
   quan::stm32::flash::symtab_entry_t constexpr names[] = {

      #define EE_SYMTAB_ENTRY(Name, CheckFun,Info, Readonly) { \
         #Name, \
         quan::stm32::flash::type_to_flash_id<flash_variable_type::Name >::value ,\
         CheckFun, \
         Info ,\
         Readonly \
       }

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

      #undef EE_SYMTAB_ENTRY
   };

   //instantiate the application symbol table
   app_symtab_t app_symtab;

} // namespace

// implement functional access to the symtab
quan::stm32::flash::symtab_entry_t const * app_symtab_t::get_symbol_table()const
{
   return names;
}

// implement get size of symtab
uint16_t app_symtab_t::get_symtable_size() const
{
   return sizeof (names)/sizeof (quan::stm32::flash::symtab_entry_t);
}

// implement global function to access the symboltable
quan::stm32::flash::symbol_table const & quan::stm32::flash::get_app_symbol_table()
{
   return app_symtab;
}

// move all this stuff.........................
// defined in symbology.cpp
bool init_values_from_flash();

// if returns false
// then something bad happened
// then try the report_errors function once you have a user i/o (e.g in mavlink_task startup
// or maybe should create a fail task
bool initialise_flash()
{
  if (!quan::stm32::flash::get_app_symbol_table().init()){
      return false;
  }
  return init_values_from_flash();
}
 