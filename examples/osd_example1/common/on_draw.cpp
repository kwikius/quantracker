#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "on_draw.hpp"
#include "osd.hpp"
#include "symbology.hpp"

using namespace quan::uav::osd;

void draw_gps_state();

namespace {
  int count = 0;
}

void quan::uav::osd::on_draw()
{
   if ( osd_show_compass() == true){
      draw_compass();
   }
   if (osd_show_afcl_horizon() == true){
      draw_artificial_horizon();
   }
   draw_batteries();
   if (home_position_is_set() == true){
      draw_altitude();
      draw_home();
   } else{ // home position
      if ( position_is_good()){
         if ( ++count == 250){ // 5 secs
            auto const ap = get_aircraft_position();
            set_home_position(ap);
         }
      }else{
         draw_gps_state();
      }
   }
}