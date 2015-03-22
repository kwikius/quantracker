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

/*
 Flash variables implementation.
 Flash variables of various types can be defined.

*/

#include <cstring>
#include <quan/three_d/vect.hpp>
#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_convert/vect3f.hpp>
#include <quan/stm32/flash/flash_convert/vect3i32.hpp>
#include <quan/stm32/flash/flash_convert/bool.hpp>
#include <quan/stm32/flash/flash_convert/float.hpp>
#include <quan/stm32/flash/flash_convert/int.hpp>
#include <quan/stm32/detail/flash.hpp>
#include <quan/stm32/flash/flash_error.hpp>

namespace {

   // The enum holds numeric ids , one for each different type in the flash vars 
   // In practise can prob use less types 1 int and 1 uint say
   // (TODO redo templates to reuse common stuff and save code bloat)
   // Comment out which you dont need
   enum flash_type_tags { 
         Bool 
         ,Float
         ,Int8
         ,Int16
         ,Int32
         ,Uint8
         ,Uint16
         ,Uint32
         ,Vect3I32  
         ,Vect3F
         ,NumTypeTags
     }; // Use NumTypeTags to check array sizes etc

   // use these local templates to map types to ids and vice versa
   template <typename T> struct type_to_flash_id;
   template <uint32_t id> struct flash_id_to_type;

    //--------- for Bool
   template <> struct type_to_flash_id<bool> {
      static constexpr uint32_t value = flash_type_tags::Bool;
   };
   template <> struct flash_id_to_type<flash_type_tags::Bool>{
      typedef bool type;
   };

   //--------- for Float
   template <> struct type_to_flash_id<float> {
      static constexpr uint32_t value = flash_type_tags::Float;
   };
   template <> struct flash_id_to_type<flash_type_tags::Float>{
      typedef float type;
   };

    //--------- for Int8
   template <> struct type_to_flash_id<int8_t> {
      static constexpr uint32_t value = flash_type_tags::Int8;
   };
   template <> struct flash_id_to_type<flash_type_tags::Int8>{
      typedef int8_t type;
   };

    //--------- for Int16
   template <> struct type_to_flash_id<int16_t> {
      static constexpr uint32_t value = flash_type_tags::Int16;
   };
   template <> struct flash_id_to_type<flash_type_tags::Int16>{
      typedef int16_t type;
   };
   
    //--------- for Int32
   template <> struct type_to_flash_id<int32_t> {
      static constexpr uint32_t value = flash_type_tags::Int32;
   };
   template <> struct flash_id_to_type<flash_type_tags::Int32>{
      typedef int32_t type;
   };

    //--------- for Uint8
   template <> struct type_to_flash_id<uint8_t> {
      static constexpr uint32_t value = flash_type_tags::Uint8;
   };
   template <> struct flash_id_to_type<flash_type_tags::Uint8>{
      typedef uint8_t type;
   };

    //--------- for Uint16
   template <> struct type_to_flash_id<uint16_t> {
      static constexpr uint32_t value = flash_type_tags::Uint16;
   };
   template <> struct flash_id_to_type<flash_type_tags::Uint16>{
      typedef uint16_t type;
   };
   
    //--------- for Uint32
   template <> struct type_to_flash_id<uint32_t> {
      static constexpr uint32_t value = flash_type_tags::Uint32;
   };
   template <> struct flash_id_to_type<flash_type_tags::Uint32>{
      typedef uint32_t type;
   };

   //--------- for Vect3I32
   template <> struct type_to_flash_id<quan::three_d::vect<int32_t> > {
      static constexpr uint32_t value = flash_type_tags::Vect3I32;
   };
   template <> struct flash_id_to_type<flash_type_tags::Vect3I32>{
      typedef quan::three_d::vect<int32_t> type;
   };

   //--------- for Vect3F
   template <> struct type_to_flash_id<quan::three_d::vect<float> > {
      static constexpr uint32_t value = flash_type_tags::Vect3F;
   };
   template <> struct flash_id_to_type<flash_type_tags::Vect3F>{
      typedef quan::three_d::vect<float> type;
   };
  
}

namespace quan{ namespace stm32{ namespace flash{

      // per app ids for types. Since types may be arranged different ways this is 
      // for the app to define as here
      // creeky but works!

      // implement the template function
      template <typename T> uint32_t get_flash_typeid(){ return get_flash_typeid_impl<T>::apply();}

      // provide specialisations  done like this so that order change is picked up
      uint32_t quan::stm32::flash::get_flash_typeid_impl<bool>::apply() 
         {return type_to_flash_id<bool>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<float>::apply() 
         {return type_to_flash_id<float>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<int8_t>::apply() 
         {return type_to_flash_id<int8_t>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<int16_t>::apply() 
         {return type_to_flash_id<int16_t>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<int32_t>::apply() 
         {return type_to_flash_id<int32_t>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<uint8_t>::apply() 
         {return type_to_flash_id<uint8_t>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<uint16_t>::apply() 
         {return type_to_flash_id<uint16_t>::value;}
      uint32_t quan::stm32::flash::get_flash_typeid_impl<uint32_t>::apply() 
         {return type_to_flash_id<uint32_t>::value;}

      uint32_t quan::stm32::flash::get_flash_typeid_impl<quan::three_d::vect<int32_t> >::apply() 
         {return type_to_flash_id<quan::three_d::vect<int32_t> >::value;}

      uint32_t quan::stm32::flash::get_flash_typeid_impl<quan::three_d::vect<float> >::apply() 
         {return type_to_flash_id<quan::three_d::vect<float> >::value;}
     
      // and these are explicit instantiations of these functions for the linker to find 
      template  uint32_t get_flash_typeid<bool>();
      template  uint32_t get_flash_typeid<float>();
      template  uint32_t get_flash_typeid<int8_t>();
      template  uint32_t get_flash_typeid<int16_t>();
      template  uint32_t get_flash_typeid<int32_t>();
      template  uint32_t get_flash_typeid<uint8_t>();
      template  uint32_t get_flash_typeid<uint16_t>();
      template  uint32_t get_flash_typeid<uint32_t>();
      template  uint32_t get_flash_typeid<quan::three_d::vect<float> >();
      template  uint32_t get_flash_typeid<quan::three_d::vect<int32_t> >();
}}}

namespace {

   // The symbol table for this app. Provides the implementation of the 
   // various data required by the symbol table
   struct app_symtab_t : quan::stm32::flash::symbol_table {
      app_symtab_t() {}
      ~app_symtab_t() {}
      uint16_t get_symbol_storage_size (int32_t symbol_index) const;
      uint16_t get_symtable_size() const;
      int32_t get_index( const char* const symbol_name) const;
      const char* get_name (int32_t symbol_index)const;
      const char* get_info (int32_t symbol_index) const;
      bool get_readonly_status(int32_t symbol_index,bool & result)const;
      pfn_text_to_bytestream get_text_to_bytestream_fun( int32_t symbol_index) const;
      pfn_bytestream_to_text get_bytestream_to_text_fun( int32_t symbol_index) const;
      bool get_typeid(int32_t symbol_index, uint32_t & dest) const;
      bool init() const;
      pfn_check_function get_check_function(int32_t symbol_index)const;
   };
//################### 


   // An Array of function pointers 
   // for converting a type as text to a bytestream
   // indexed by id in the flash_type_tags enum
   // The order and number 
   // must be the same as the flash_type_tags enum above
   // The use of the flash_id_to_type index hopefully helps!
   constexpr quan::stm32::flash::symbol_table::pfn_text_to_bytestream text_to_bytestream[] =
   {
      &quan::stm32::flash::flash_convert<flash_id_to_type<0>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<1>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<2>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<3>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<4>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<5>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<6>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<7>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<8>::type>::text_to_bytestream
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<9>::type>::text_to_bytestream
   };
   static_assert ((sizeof(text_to_bytestream) 
         / sizeof(quan::stm32::flash::symbol_table::pfn_text_to_bytestream)) 
      == flash_type_tags::NumTypeTags,"text_to_bytestream invalid number of elements");

   // Array describing the size of a type in flash
   // indexed by id in the flash_type_tags enum
   // the order and number must be the same as the flash_type_tags enum
   // above.
   // There must be a separate entry for each different flash type
   constexpr uint32_t type_tag_to_size[] =
   {
      sizeof (flash_id_to_type<0>::type)
      ,sizeof (flash_id_to_type<1>::type) 
      ,sizeof (flash_id_to_type<2>::type)
      ,sizeof (flash_id_to_type<3>::type)
      ,sizeof (flash_id_to_type<4>::type) 
      ,sizeof (flash_id_to_type<5>::type)
      ,sizeof (flash_id_to_type<6>::type)
      ,sizeof (flash_id_to_type<7>::type) 
      ,sizeof (flash_id_to_type<8>::type)
      ,sizeof (flash_id_to_type<9>::type)
   };

   static_assert ((sizeof(type_tag_to_size) / sizeof(uint32_t))
      == flash_type_tags::NumTypeTags,"type_tag_to_size invalid number of elements");
    
   // array of function pointers to convert a bytestream to text (user represntation)
   // indexed by id in the flash_type_tags enum
   // the order and number must be the same as the flash_type_tags enum
   // Can be by type, not by object, since
   // it is assumed that only valid object values have been stored in Flash.
   constexpr quan::stm32::flash::symbol_table::pfn_bytestream_to_text bytestream_to_text[] =
   {
      &quan::stm32::flash::flash_convert<flash_id_to_type<0>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<1>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<2>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<3>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<4>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<5>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<6>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<7>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<8>::type>::bytestream_to_text
      ,&quan::stm32::flash::flash_convert<flash_id_to_type<9>::type>::bytestream_to_text
   };
   static_assert ((sizeof(bytestream_to_text) 
         / sizeof(quan::stm32::flash::symbol_table::pfn_bytestream_to_text)) 
      == flash_type_tags::NumTypeTags,"bytestream_to_text invalid number of elements");

// ####################### access to types via the lookup name ####################
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
            type_to_flash_id<flash_variable_type::Name >::value ,\
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
   constexpr uint16_t get_type_index (int32_t symbol_index)
   {
      return names[static_cast<uint16_t>(symbol_index)].type_tag;
   }
   
   // get size of a type by index
   // requires valid type_index -- not checked
   constexpr uint16_t get_type_size (uint16_t typeidx)
   {
      return type_tag_to_size[typeidx];
   }
    
   //instantiate the application symbol table
   app_symtab_t app_symtab;

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

// requires valid symbol_index -- not checked
app_symtab_t::pfn_text_to_bytestream app_symtab_t::get_text_to_bytestream_fun (int32_t symbol_index)const
{
   return text_to_bytestream[get_type_index (symbol_index) ];
}

// coming out data assumed ok. add checks
// requires valid symbol_index -- not checked
app_symtab_t::pfn_bytestream_to_text app_symtab_t::get_bytestream_to_text_fun (int32_t symbol_index)const
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
      quan::user_message ("invalid flash page\n");
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
   
     // quan::user_message ("unwritten flash detected... initialising\n");
     
      if (! quan::stm32::flash::detail::erase (1)) {
         quan::error (fn_any,quan::detail::stm32_flash_erase_page_failed);
         return false ;
      }
      if (! quan::stm32::flash::detail::erase (2)) {
         quan::error(fn_any,quan::detail::stm32_flash_erase_page_failed);
         return false;
      }
      //quan::user_message ("...flash erased OK\n");
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
 