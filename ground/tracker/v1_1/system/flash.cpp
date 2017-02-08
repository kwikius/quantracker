/*
 Copyright (c) 2013 -2017 Andy Little 

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
#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_error.hpp>

/*
 A custom flash symbol table.
 The file can be used as a template to create your own symbol table.
 All that is necessary is to change the variable definitions in the symbol table
 and optionally add a range check function to the flash variable

*/

// the flash symbol table is placed in an anonymous namespace
// to reduce the amount of global names
namespace {

   // The symbol table for this app. 
   struct app_symtab_t : quan::stm32::flash::symbol_table {
      app_symtab_t() {}
      ~app_symtab_t() {}
      uint16_t get_symtable_size() const;
      quan::stm32::flash::symtab_entry_t const * get_symbol_table()const;
   };

// ####################### access to types via the lookup name ####################
   // This structure is used to map the type of each flash variable to its lookup name
   // for use by the EE_SYMTAB_ENTRY macro below. so for example,
   // the variable looked up as "show_home", has a typedef named show_home, aliasing a bool.
   // The order isnt important except for neatness !
   struct flash_variable_type {

      typedef float                          pan_motor_omega_per_mV;
      typedef float                          pan_motor_kP;
      typedef float                          pan_motor_kD;
      typedef float                          pan_motor_minDC;
      typedef float                          pan_motor_kSpike;
      typedef uint32_t                       pan_motor_encoder_counts_rev; 

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

//####### The flash_variables symtable itself ###########################
 
   quan::stm32::flash::symtab_entry_t constexpr flash_variables_symtab[] = {

      #define EE_SYMTAB_ENTRY(Name, CheckFun,Info, Readonly) { \
         #Name, \
         quan::stm32::flash::type_to_flash_id<flash_variable_type::Name >::value ,\
         CheckFun, \
         Info ,\
         Readonly \
       }
       EE_SYMTAB_ENTRY(pan_motor_omega_per_mV,nop_check,"pan_motor_omega_per_mV",false)
       ,EE_SYMTAB_ENTRY(pan_motor_kP,nop_check,"pan_motor_kP",false)
       ,EE_SYMTAB_ENTRY(pan_motor_kD,nop_check,"pan_motor_kD",false)
       ,EE_SYMTAB_ENTRY(pan_motor_minDC,nop_check,"pan_motor_minDC",false)
       ,EE_SYMTAB_ENTRY(pan_motor_minDC,nop_check,"pan_motor_kSpike",false)
      #undef EE_SYMTAB_ENTRY
   };

   //instantiate the application symbol table
   app_symtab_t app_symtab;

} // namespace

bool initialise_flash()
{
  if (! quan::stm32::flash::get_app_symbol_table().init()){
      return false;
  }
  return true;
  // add bool init flash values
}

// implement get size of symtab
uint16_t app_symtab_t::get_symtable_size() const
{
   return sizeof (flash_variables_symtab)/sizeof (quan::stm32::flash::symtab_entry_t);
}

// implement functional access to the symtab
quan::stm32::flash::symtab_entry_t const * app_symtab_t::get_symbol_table()const
{
   return flash_variables_symtab;
}

// implement global function to access the symboltable
quan::stm32::flash::symbol_table const & quan::stm32::flash::get_app_symbol_table()
{
   return app_symtab;
}
