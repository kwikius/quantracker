/*
 Copyright (c) 2012 - 2015 Andy Little

(
  Some parts of this work are based on: 
  http://code.google.com/p/arducam-osd/source/browse/trunk/ArduCAM_OSD/MAVLink.ino
  Copyright (c) 2011. Sandro Benigno
)

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

#include <cstdint>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "mavlink.hpp"

#include "aircraft.hpp"
#include "resources.hpp"


// see qgroundcontrol.org/dev/mavlink_onboard_integration_tutorial
// Not quite sure what values these should be though...
 // think they are don't care here
static constexpr uint8_t mavlink_sysid = 1;  // system id
static constexpr uint8_t mavlink_compid = 1; // component id
mavlink_system_t mavlink_system = {mavlink_sysid,mavlink_compid}; // 

void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
 #if  (QUAN_OSD_BOARD_TYPE == 4)
   mavlink_tx_rx_task::put(ch);
#else
  posdata_tx_rx_task::put(ch);
#endif
}

void signal_new_heartbeat();

namespace{

   uint8_t  apm_mav_type;
   uint8_t  apm_mav_system; 
   uint8_t  apm_mav_component;

   // prob add this to the_aircraft structure
   bool  aircraft_home_set = false;
   //quan::time_<float>::s aircraft_home_set_time;

   struct mav_sr_t{
      uint8_t stream_number;
      uint16_t rate;
   };

// The same as the minimosd

   mav_sr_t MAVStreams[] = {
      {MAV_DATA_STREAM_RAW_SENSORS,0x02},
      {MAV_DATA_STREAM_EXTENDED_STATUS,0x02},
      {MAV_DATA_STREAM_RC_CHANNELS,0x05},
      {MAV_DATA_STREAM_POSITION,0x02},
      {MAV_DATA_STREAM_EXTRA1, 0x05},
      {MAV_DATA_STREAM_EXTRA2,0x02}
   };

   void request_mavlink_rate(uint8_t system, uint8_t component, uint8_t stream_number, uint16_t rate)
   {
        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,system, component, stream_number, rate, 1);
   }

   bool rate_request_sent_flag = false;

   void  request_mavlink_rates()
   {
     static const size_t  NumStreams = sizeof(MAVStreams) / sizeof(mav_sr_t);
     for (size_t i=0; i < NumStreams; ++i) {
         request_mavlink_rate(apm_mav_system, apm_mav_component,MAVStreams[i].stream_number, MAVStreams[i].rate);
     }
     rate_request_sent_flag = true;
   }

  void do_mavlink_heartbeat(mavlink_message_t* pmsg);
  void do_mavlink_sys_status(mavlink_message_t *pmsg);
#ifndef MAVLINK10
  void do_mavlink_gps_raw(mavlink_message_t * pmsg);
  void do_mavlink_gps_status(mavlink_message_t * pmsg);
#else
  void do_mavlink_gps_raw_int(mavlink_message_t * pmsg);
#endif
  void do_mavlink_vfr_hud(mavlink_message_t * pmsg);
  void do_mavlink_attitude(mavlink_message_t * pmsg);

   void read_mavlink(void * param)
   {
      mavlink_message_t msg; 
      {  // init msg to silence gcc warnings
         msg.msgid  = 255; // TODO should be initialised for rx?
         msg.compid = 255;
         msg.sysid  = 255;
         constexpr uint32_t payload_64_size = ( sizeof(msg.payload64) / sizeof(uint64_t));
         for ( uint32_t i = 0U; i < payload_64_size; ++i){
            msg.payload64[i] = 0UL;
         }
      }
      mavlink_status_t status ;

      the_aircraft.mutex_init();
#if  (QUAN_OSD_BOARD_TYPE == 4)
      mavlink_tx_rx_task::enable();
#else
      posdata_tx_rx_task::enable();
#endif
      for(;;){
  #if  (QUAN_OSD_BOARD_TYPE == 4)    
         uint8_t ch =  mavlink_tx_rx_task::get();
  #else
         uint8_t ch =  posdata_tx_rx_task::get();
  #endif

         if(mavlink_parse_char(MAVLINK_COMM_0, ch, &msg, &status)) {
            
            switch(msg.msgid) {
               case MAVLINK_MSG_ID_HEARTBEAT:
                  do_mavlink_heartbeat(&msg);
                  if ( rate_request_sent_flag == false){
                     request_mavlink_rates();
                  }
               break;
               case MAVLINK_MSG_ID_SYS_STATUS:
                  do_mavlink_sys_status(&msg);
               break;
   #ifndef MAVLINK10 
               case MAVLINK_MSG_ID_GPS_RAW:
                  do_mavlink_gps_raw(&msg);
               break;
               case MAVLINK_MSG_ID_GPS_STATUS:
                  do_mavlink_gps_status(&msg);
               break;
   #else
               case MAVLINK_MSG_ID_GPS_RAW_INT:
                  do_mavlink_gps_raw_int(&msg);
               break;
   #endif          
               case MAVLINK_MSG_ID_VFR_HUD:
                  do_mavlink_vfr_hud(&msg);
               break;
               case MAVLINK_MSG_ID_ATTITUDE:
                  do_mavlink_attitude(&msg);
               break;
               default:
               break;
            }
           // packet_drops += status.packet_rx_drop_count;
           // parse_error += status.parse_error;
         }
      }
   }

  void do_mavlink_heartbeat(mavlink_message_t* pmsg)
  {    
      apm_mav_system    = pmsg->sysid;
      apm_mav_component = pmsg->compid;
      apm_mav_type      = mavlink_msg_heartbeat_get_type(pmsg);

#ifdef MAVLINK10 
      the_aircraft.mutex_acquire();            
         the_aircraft.custom_mode = mavlink_msg_heartbeat_get_custom_mode(pmsg);
         the_aircraft.nav_mode = 0;
      the_aircraft.mutex_release(); 
#endif            
     signal_new_heartbeat();
   }

   void do_mavlink_sys_status(mavlink_message_t *pmsg)
   {
      the_aircraft.mutex_acquire();
#ifndef MAVLINK10  
            
         the_aircraft.battery_voltage 
         = quan::voltage_<float>::mV{mavlink_msg_sys_status_get_vbat(pmsg) };
      //  = mavlink_msg_sys_status_get_vbat(pmsg)/ 1000.f;
         the_aircraft.custom_mode = mavlink_msg_sys_status_get_mode(pmsg);
         the_aircraft.nav_mode = mavlink_msg_sys_status_get_nav_mode(pmsg);
#else
         the_aircraft.battery_voltage
            = quan::voltage_<float>::mV{mavlink_msg_sys_status_get_voltage_battery(pmsg)};
        // = mavlink_msg_sys_status_get_voltage_battery(pmsg) / 1000.f;
#endif            
         the_aircraft.battery_remaining = mavlink_msg_sys_status_get_battery_remaining(pmsg);
      the_aircraft.mutex_release();
   }

   
#ifndef MAVLINK10
#error wrong version
   void do_mavlink_gps_raw(mavlink_message_t * pmsg)
   {
      the_aircraft.mutex_acquire();
         the_aircraft.location.lat = quan::angle_<float>::deg10e7{mavlink_msg_gps_raw_get_lat(pmsg)* 1.e7f};
       //  the_aircraft.location.lat = mavlink_msg_gps_raw_get_lat(pmsg) * 1.e7f;
         the_aircraft.location.lon = quan::angle_<float>::deg10e7{mavlink_msg_gps_raw_get_lon(pmsg) * 1.e7f};
         // the_aircraft.location.lon = mavlink_msg_gps_raw_get_lon(pmsg) * 1.e7f;
         the_aircraft.gps.fix_type = mavlink_msg_gps_raw_get_fix_type(pmsg);
      the_aircraft.mutex_release();
   }
   void do_mavlink_gps_status(mavlink_message_t * pmsg)
   {
      the_aircraft.mutex_acquire();
         the_aircraft.gps.num_sats = mavlink_msg_gps_status_get_satellites_visible(pmsg);
      the_aircraft.mutex_release();
   }
#else

  void do_mavlink_gps_raw_int(mavlink_message_t * pmsg)
   {
   the_aircraft.mutex_acquire();
      the_aircraft.gps.time_stamp = quan::time_<uint64_t>::us{mavlink_msg_gps_raw_int_get_time_usec(pmsg)};
      //  the_aircraft.location.gps_time_stamp = mavlink_msg_gps_raw_int_get_time_usec(pmsg);
      the_aircraft.gps.lat = quan::angle_<int32_t>::deg10e7{ mavlink_msg_gps_raw_int_get_lat(pmsg) };
      the_aircraft.location.lat = the_aircraft.gps.lat;
      // the_aircraft.location.gps_lat =  mavlink_msg_gps_raw_int_get_lat(pmsg) ;
      the_aircraft.gps.lon = quan::angle_<int32_t>::deg10e7{ mavlink_msg_gps_raw_int_get_lon(pmsg) };
      the_aircraft.location.lon = the_aircraft.gps.lon;
      //  the_aircraft.location.gps_lon =  mavlink_msg_gps_raw_int_get_lon(pmsg) ;
      the_aircraft.gps.alt = quan::length_<int32_t>::mm{ mavlink_msg_gps_raw_int_get_alt(pmsg) };
      the_aircraft.location.alt = the_aircraft.gps.alt;
      // the_aircraft.location.gps_alt = mavlink_msg_gps_raw_int_get_alt(pmsg);
      the_aircraft.gps.vog = quan::velocity_<uint16_t>::cm_per_s{ mavlink_msg_gps_raw_int_get_vel(pmsg)};
      //  the_aircraft.location.gps_vog =  mavlink_msg_gps_raw_int_get_vel(pmsg);
      the_aircraft.gps.cog =  quan::angle_<uint16_t>::cdeg{mavlink_msg_gps_raw_int_get_cog(pmsg)};
      //  the_aircraft.location.gps_cog =  mavlink_msg_gps_raw_int_get_cog(pmsg);
      the_aircraft.gps.hdop = quan::length_<uint16_t>::cm{ mavlink_msg_gps_raw_int_get_eph(pmsg) };
      //  the_aircraft.location.gps_hdop = mavlink_msg_gps_raw_int_get_eph(pmsg);
      the_aircraft.gps.vdop = quan::length_<int32_t>::cm{ mavlink_msg_gps_raw_int_get_epv(pmsg) };
      // the_aircraft.location.gps_vdop =  mavlink_msg_gps_raw_int_get_epv(pmsg);
      the_aircraft.gps.fix_type = mavlink_msg_gps_raw_int_get_fix_type(pmsg);
      the_aircraft.gps.num_sats = mavlink_msg_gps_raw_int_get_satellites_visible(pmsg);
      
      the_aircraft.mutex_release();

      if ((aircraft_home_set == false) && ( the_aircraft.gps.fix_type > 1)){
         the_aircraft.home_location = the_aircraft.location;
         aircraft_home_set = true;
      }       
   }
#endif

   void do_mavlink_vfr_hud(mavlink_message_t * pmsg)
   {
      the_aircraft.mutex_acquire();
         // mavlink_msg_vfr_hud_get_groundspeed retirns val in m/s
         the_aircraft.groundspeed 
           = quan::velocity_<float>::m_per_s{mavlink_msg_vfr_hud_get_groundspeed(pmsg)};
          //  = mavlink_msg_vfr_hud_get_groundspeed(pmsg);
         // mavlink_msg_vfr_hud_get_airspeed returns val in m/s
         the_aircraft.airspeed 
            = quan::velocity_<float>::m_per_s{mavlink_msg_vfr_hud_get_airspeed(pmsg)};
           //=  mavlink_msg_vfr_hud_get_airspeed(pmsg);
//##############################
       // turns out this is an integer version of the ahrs yaw param in degrees
       // better to use the yaw param sent in the attitude message
       // the_aircraft.heading = quan::angle_<float>::deg{mavlink_msg_vfr_hud_get_heading(pmsg)};
       //  the_aircraft.heading = mavlink_msg_vfr_hud_get_heading(pmsg);
//################################################################
         the_aircraft.throttle = mavlink_msg_vfr_hud_get_throttle(pmsg) / 100.f;

         if((the_aircraft.throttle > 1.f) && (the_aircraft.throttle < 1.5f)){
            the_aircraft.throttle = 1.f;//Temporary fix for ArduPlane 2.28
         }else {
            if((the_aircraft.throttle < 0.f) || (the_aircraft.throttle > 1.5f)){
               the_aircraft.throttle = 0.f;//Temporary fix for ArduPlane 2.28
             }
         }
         // mavlink_msg_vfr_hud_get_alt returns height in m
         // baroalt
         the_aircraft.baro_alt = quan::length_<float>::m{mavlink_msg_vfr_hud_get_alt(pmsg)};
        // the_aircraft.location.baro_alt = mavlink_msg_vfr_hud_get_alt(pmsg);
      the_aircraft.mutex_release();
   }

   void do_mavlink_attitude(mavlink_message_t * pmsg)
   {
      the_aircraft.mutex_acquire();
         // static const float pi = 3.141592653589793238462643383279502884197;
         // static const float rad_to_deg = 180.f / pi;
         the_aircraft.attitude.pitch = quan::angle_<float>::rad{mavlink_msg_attitude_get_pitch(pmsg)};
         // the_aircraft.attitude.pitch = mavlink_msg_attitude_get_pitch(pmsg) * rad_to_deg;
         the_aircraft.attitude.roll = quan::angle_<float>::rad{mavlink_msg_attitude_get_roll(pmsg)};
         // the_aircraft.attitude.roll = mavlink_msg_attitude_get_roll(pmsg)* rad_to_deg;
         auto yaw = quan::angle_<float>::rad{mavlink_msg_attitude_get_yaw(pmsg)};
         the_aircraft.attitude.yaw = yaw;
          // therefore could dump this var as is same as heading
         the_aircraft.heading = yaw;
         //the_aircraft.attitude.yaw = mavlink_msg_attitude_get_yaw(pmsg) * rad_to_deg;
      the_aircraft.mutex_release();
   }

   char dummy_param  =0;
   TaskHandle_t task_handle = NULL;

}//namespace

void create_mavlink_task()
{
   xTaskCreate(
         read_mavlink,"read_mavlink", 
         512,
         &dummy_param,
         task_priority::mavlink,
         &task_handle);
}


