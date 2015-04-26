
#ifndef QUANTRACKER_AIR_AIRCRAFT_HPP_INCLUDED
#define QUANTRACKER_AIR_AIRCRAFT_HPP_INCLUDED

/*
 Copyright (c) 2012 - 2015 Andy Little 

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

#if !defined QUANTRACKER_AIR_OSD_PC_SIM_MODE
#include <FreeRTOS.h>
#include <semphr.h>
#endif

#ifdef QUAN_STM32F4
#include <quan/stm32f4/config.hpp>
#endif
#include <quan/time.hpp>
#include <quan/length.hpp>
#include <quan/velocity.hpp>
#include <quan/voltage.hpp>
#include <quan/current.hpp>
#include <quan/angle.hpp>
#include <quan/uav/attitude.hpp>
#include <quan/uav/osd/api.hpp>


struct gps_t{
   uint8_t num_sats;
   uint8_t fix_type;
   bool  has_home;
   quan::time_<uint64_t>::us                             time_stamp;
   quan::angle_<int32_t>::deg10e7                        lat;  
   quan::angle_<int32_t>::deg10e7                        lon; 
   quan::velocity_<uint16_t>::cm_per_s                   vog;  
   quan::angle_<uint16_t>::cdeg                          cog;
   quan::length_<int32_t>::mm                            alt;  
   quan::length_<uint16_t>::cm                           hdop;
   quan::length_<uint16_t>::cm                           vdop; 

};

/*
struct attitude_t{
   quan::angle_<float>::deg            pitch;                  
   quan::angle_<float>::deg            roll;                   
   quan::angle_<float>::deg            yaw; 
   attitude_t():pitch{0},roll{0},yaw{0}{}
};
*/

struct aircraft{
  // location_t    
   quan::uav::osd::position_type       location;
   quan::uav::osd::position_type       home_location;
   quan::uav::osd::attitude_type       attitude;
   float                               throttle;                  
   quan::angle_<float>::deg            heading;              
   quan::velocity_<float>::m_per_s     airspeed;            
   quan::velocity_<float>::m_per_s     groundspeed;            
   quan::voltage_<float>::V            battery_voltage;  
   quan::current_<float>::A            battery_current; 
   float                               battery_remaining; 
   quan::length_<float>::m             baro_alt;
   uint16_t                            nav_mode;
   uint8_t                             custom_mode;
   uint8_t                             base_mode;
   gps_t                               gps;
   uint16_t                            rc_raw_chan[8];
   uint16_t                            rc_raw_rssi;
                                          
   aircraft():
      throttle{0},
      heading{0},
      airspeed{0},
      groundspeed{0},
      battery_voltage{0},
      battery_current{0},
      battery_remaining{0},
      nav_mode{0},
      custom_mode{0},
      base_mode{0},
      rc_raw_chan{0,0,0,0,0,0,0,0},
      rc_raw_rssi{0}
#if !defined QUANTRACKER_AIR_OSD_PC_SIM_MODE
      ,m_mutex{0}
#endif
   {}
#if !defined QUANTRACKER_AIR_OSD_PC_SIM_MODE
   void mutex_init();
   void mutex_acquire();
   void mutex_release();
   private:
      SemaphoreHandle_t m_mutex;
#endif
};

extern aircraft the_aircraft;

#endif // QUANTRACKER_AIR_AIRCRAFT_HPP_INCLUDED
