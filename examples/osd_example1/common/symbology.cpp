
#include <quan/uav/osd/pal_ntsc_pos.hpp>
#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_convert.hpp>
#include "symbology.hpp"

namespace {
   quan::uav::osd::pal_ntsc_pos  display_home_position = {-160,70,60};
   quan::uav::osd::pal_ntsc_pos  display_compass_position = {50,-115,-65};
}

quan::uav::osd::pxp_type get_display_home_position()
{
   return display_home_position;
}

quan::uav::osd::pxp_type get_display_compass_position()
{
   return display_compass_position;
}

bool init_values_from_flash()
{
   auto & symtab = quan::stm32::flash::get_app_symbol_table();
   // symbol may be undef so check first
   if ( symtab.is_defined("display_home_pos")){
       quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("display_home_pos",v)){
         display_home_position.set_xpos(v.x);
         display_home_position.set_ypos(v.y,v.z);
      }
   }

   if ( symtab.is_defined("display_compass_pos")){
      quan::three_d::vect<int32_t> v;
      if ( quan::stm32::flash::get_flash_value("display_compass_pos",v)){
         display_compass_position.set_xpos(v.x);
         display_compass_position.set_ypos(v.y,v.z);
      }
   }
   return true;
}