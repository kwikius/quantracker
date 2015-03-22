
#include <quan/uav/osd/pal_ntsc_pos.hpp>
#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_convert.hpp>
#include "symbology.hpp"

namespace {
   // these are the defaults
   // will be overwritten if customised in flash
   bool want_home = true;
   quan::uav::osd::pal_ntsc_pos osd_home_position = {-160,70,60};
   bool want_compass = true;
   quan::uav::osd::pal_ntsc_pos osd_compass_position = {0,-115,-65};
   bool want_gps_no_fix = true;
   quan::uav::osd::pal_ntsc_pos osd_gps_no_fix_position = {-160,70,50};
   bool want_altitude = true;
   quan::uav::osd::pal_ntsc_pos osd_altitude_position = {-160,50,30};
   bool want_afcl_horizon = true;
   quan::uav::osd::angle_type  osd_afcl_horiz_pitch_adj{0}; 

}

bool osd_show_altitude() { return want_altitude;}
bool osd_show_home() { return want_home;}
bool osd_show_compass() { return want_compass;}
bool osd_show_gps_no_fix() { return want_gps_no_fix;}
bool osd_show_afcl_horizon() { return want_afcl_horizon;}

quan::uav::osd::pxp_type get_osd_altitude_position()
{
   return  osd_altitude_position;
}

quan::uav::osd::pxp_type get_osd_home_position()
{
   return osd_home_position;
}

quan::uav::osd::pxp_type get_osd_compass_position()
{
   return osd_compass_position;
}

quan::uav::osd::pxp_type get_osd_gps_no_fix_position()
{
   return osd_gps_no_fix_position;
}

quan::uav::osd::angle_type osd_get_afcl_horizon_pitch_adj()
{
   return osd_afcl_horiz_pitch_adj;
}

// if returns false look to the report errors mechanism to find the error
// When called there aint no user io
// so will need to check for and report  errors in mavlink_task at startup
bool init_values_from_flash()
{
   auto & symtab = quan::stm32::flash::get_app_symbol_table();
   // symbol may be valid but not yet defined by user
   
   if ( symtab.is_symbol_name_defined_in_flash("show_home")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_home",v)){
         want_home = v;
      }
   }
   if ( symtab.is_symbol_name_defined_in_flash("osd_home_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_home_pos",v)){
         osd_home_position.set_xpos(v.x);
         osd_home_position.set_ypos(v.y,v.z);
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("show_compass")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_compass",v)){
         want_compass = v;
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("osd_compass_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_compass_pos",v)){
         osd_compass_position.set_xpos(v.x);
         osd_compass_position.set_ypos(v.y,v.z);
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("show_altitude")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_altitude",v)){
         want_altitude = v;
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("osd_altitude_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_altitude_pos",v)){
         osd_altitude_position.set_xpos(v.x);
         osd_altitude_position.set_ypos(v.y,v.z);
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("show_gps_no_fix")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_gps_no_fix",v)){
         want_gps_no_fix = v;
      }
   }
 
   if ( symtab.is_symbol_name_defined_in_flash("osd_gps_no_fix_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("osd_gps_no_fix_pos",v)){
         osd_gps_no_fix_position.set_xpos(v.x);
         osd_gps_no_fix_position.set_ypos(v.y,v.z);
      }
   }

   if ( symtab.is_symbol_name_defined_in_flash("show_afcl_horizon")){
      bool v = false;
      if ( quan::stm32::flash::get_flash_value("show_afcl_horizon",v)){
         want_afcl_horizon = v;
      }
   }

    if ( symtab.is_symbol_name_defined_in_flash("osd_afcl_horizon_pitch_adj")){
      int16_t v;
      if ( quan::stm32::flash::get_flash_value("osd_afcl_horizon_pitch_adj",v)){
          osd_afcl_horiz_pitch_adj = quan::uav::osd::angle_type{v};
      }
   }
 
   return true;
}