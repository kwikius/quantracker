#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "on_draw.hpp"
#include "osd.hpp"

using namespace quan::uav::osd;

void draw_gps_state();

void quan::uav::osd::on_draw()
{

   draw_compass();
   draw_artificial_horizon();
   if (home_position_is_set() == true){
      draw_altitude();
      draw_home();
   } else{ // home position
      if ( position_is_good()){
         set_home_position(get_aircraft_position());
      }else{
         draw_gps_state();
      }
   }
}