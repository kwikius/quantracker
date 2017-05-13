
/*
 Copyright (c) 2017 Andy Little 

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

#include <quan/atan2.hpp>
#include <quan/stm32/millis.hpp>
#include <quan/uav/osd/api.hpp>
#include "azimuth/servo.hpp"
#include "elevation/servo.hpp"
#include "system/resources.hpp"
#include <quan/min.hpp>

bool button_pressed(); // atomic
void clear_button_pressed(); // atomic

namespace {

   QUAN_ANGLE_LITERAL(rad)
   QUAN_ANGLE_LITERAL(deg)
   QUAN_QUANTITY_LITERAL(time,ms)
   QUAN_QUANTITY_LITERAL(length,m)

   typedef quan::uav::position<
      quan::angle::deg,
      quan::length::mm
   > local_pos_type;

   local_pos_type m_home_position;
   local_pos_type m_aircraft_position;

   enum class tracker_t{ initialising, preparing_to_track, tracking};

   tracker_t tracker_state = tracker_t::initialising;

   quan::time_<int64_t>::ms button_pressed_time = 0.0_ms;

   template <typename T>
   void filter (T & value, T const & new_value, float filter_k){
      value = new_value * filter_k + value * (1.f - filter_k);
   }

   quan::time_<int64_t>::ms last_filter_update = 0_ms;

   /*
      set weight of filter according to how long since last update.
      longer time than more weight
      If a long time than set weight to 1
   */

   float get_update_filter_k()
   {
       constexpr auto k = 0.05f / 20_ms; // gives full weight at 0.4 s
       auto const now = quan::stm32::millis();
       quan::time_<int64_t>::ms dt = now - last_filter_update;
       last_filter_update = now;
       if ( dt > quan::time_<int64_t>::ms{1000}){
          return 1.f;
       }else{
          return quan::min(k * dt,1.f);
       }
   }

   void update_aircraft_position(local_pos_type const & new_position)
   {
      float const filter_k = get_update_filter_k();
      
      filter(m_aircraft_position.lat,new_position.lat,filter_k);
      filter(m_aircraft_position.lon,new_position.lon,filter_k);
      filter(m_aircraft_position.alt,new_position.alt,filter_k);
   }

   constexpr quan::length::m radius_of_world = 6371000_m;

   //Precondition m_home_position must be initialised before calling recalc
   void recalc()
   {
      quan::angle::deg const dlat = m_aircraft_position.lat - m_home_position.lat; // sort diffence for angle
      quan::angle::deg const dlon = m_aircraft_position.lon - m_home_position.lon;

      quan::length::m const rlat = cos(abs(m_home_position.lat)) * radius_of_world;
      
      quan::length::m const dx = (dlon * rlat) / quan::angle::rad{1};
      quan::length::m const dy =  (dlat * radius_of_world) / quan::angle::rad{1};
      quan::length::m const distance = quan::sqrt(dx * dx + dy * dy);

      // if at home avoid division by zero etc
      bool const at_home = abs(distance) <= 2_m;

      quan::angle::rad elevation = (at_home)
      ? quan::angle::pi / 4  
      : unsigned_modulo(quan::atan2(m_aircraft_position.alt - m_home_position.alt, distance));

      quan::angle::rad bearing = (at_home)
      ? 0._rad
      : unsigned_modulo(quan::atan2(dx,dy));
     
      azimuth_servo::set_target(bearing);
      elevation_servo::set_elevation(elevation);
   }
}// ~namespace

void tracking_update(quan::uav::osd::norm_position_type const & pos)
{
  //NOTE: need to convert to degrees from deg10e7 here, not to rad directly else get overflow
   local_pos_type const conv_pos = pos;

   update_aircraft_position(conv_pos);

   switch (tracker_state){
      case tracker_t::initialising:
         if (button_pressed()){
            gcs_serial::write("preparing to track\n");
            azimuth_encoder::set_index(0U);
            button_pressed_time = quan::stm32::millis();
            tracker_state = tracker_t::preparing_to_track;
         }
      break;
      case tracker_t::preparing_to_track:
         if ((quan::stm32::millis() - button_pressed_time) > 1000_ms){
            m_home_position = m_aircraft_position;
            recalc();
            azimuth_servo::enable();
            elevation_servo::enable();
            clear_button_pressed();
            tracker_state = tracker_t::tracking;
            gcs_serial::write("tracking\n");
         } 
      break;
      case tracker_t::tracking:
         recalc();
      break;
      default:
      break;
   }
}

