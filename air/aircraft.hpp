
#ifndef QUANTRACKER_AIR_AIRCRAFT_HPP_INCLUDED
#define QUANTRACKER_AIR_AIRCRAFT_HPP_INCLUDED

/*
 Copyright (c) 2012 - 2013 Andy Little 

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

#ifdef __AVR__
#include <stdint.h>
#else 
#include <cstdint>
#endif
//#include <quan/stm32f4/config.hpp>
#include <quan/time.hpp>
#include <quan/length.hpp>
#include <quan/velocity.hpp>
#include <quan/voltage.hpp>
#include <quan/current.hpp>
#include <quan/angle.hpp>

struct location_t{
/*
   quan::angle_<float>::deg    lat;       
   quan::angle_<float>::deg    lon;
   quan::length_<float>::m     alt;
   location_t():lat{0},lon{0},alt{0}{}
  
*/
   quan::time_<uint64_t>::us                             gps_time_stamp;
   quan::angle_<int32_t>::deg10e7                        gps_lat;  
   quan::angle_<int32_t>::deg10e7                        gps_lon; 
   quan::velocity_<uint16_t>::cm_per_s                   gps_vog;  
   quan::angle_<uint16_t>::cdeg                          gps_cog;
   quan::length_<int32_t>::mm                            gps_alt;  
   quan::length_<uint16_t>::cm                           gps_hdop;
   quan::length_<uint16_t>::cm                           gps_vdop; 

 };

struct gps_t{
   uint8_t num_sats;
   uint8_t fix_type;
   bool  has_home;
};

struct attitude_t{
   quan::angle_<float>::deg            pitch;                  
   quan::angle_<float>::deg            roll;                   
   quan::angle_<float>::deg            yaw; 
  attitude_t():pitch{0},roll{0},yaw{0}{}
};

struct aircraft{
   location_t                          location;
  // location_t                          home_location;
   attitude_t                          attitude;
   float                               throttle;                  
   quan::angle_<float>::deg            heading;              
   quan::velocity_<float>::m_per_s     airspeed;            
   quan::velocity_<float>::m_per_s     groundspeed;            
   quan::voltage_<float>::V            battery_voltage;  
   quan::current_<float>::A            battery_current; 
   float                               battery_remaining; 
   quan::length_<float>::m             baro_alt;
  // quan::length_<float>::m             get_distance_from_home(); 
 //  quan::angle_<float>::deg            get_bearing_to_home();
   uint16_t                            nav_mode;
   uint8_t                             custom_mode;
   gps_t                               gps;
                                          
   aircraft():
      throttle{0},
      heading{0},
      airspeed{0},
      groundspeed{0},
      battery_voltage{0},
      battery_current{0},
      battery_remaining{0},
      nav_mode{0},
      custom_mode{0}
   {}
   
};

extern aircraft the_aircraft;

#endif // QUANTRACKER_AIR_AIRCRAFT_HPP_INCLUDED
