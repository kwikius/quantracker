
#include <stdlib.h>
#include <stdio.h>
#include "serial_ports.hpp"
#include <quan/config.hpp>
#include "telemetry.hpp"
#include "azimuth.hpp"
#include <quan/uav/msg_protocol.hpp>
#include "frsky_serial_port.hpp"
#include <quan/utility/fifo.hpp>
#include <quan/uav/do_checksum.hpp>
#include <quan/atan2.hpp>

telemetry::gps_position telemetry::m_home_position{quan::angle::deg{0},quan::angle::deg{0},quan::length::m{0}};
telemetry::gps_position  telemetry::m_aircraft_position{quan::angle::deg{0},quan::angle::deg{0},quan::length::m{0}};
frsky_serial_port telemetry::serial_port; 
bool telemetry::state_changed = false;

void telemetry::set_home()
{
   m_home_position = m_aircraft_position;
}

namespace {

   constexpr auto filter_k = 0.95f;

   template <typename T>
   T filter (T const & value, T const & new_value){
      return value * filter_k + new_value * ( 1 - filter_k);
   }

   telemetry::gps_position filtered_position {quan::angle::deg{0},quan::angle::deg{0},quan::length::m{0}};

  
}

void telemetry::filter_pos()
{
   filtered_position.lat = filter(filtered_position.lat,telemetry::m_aircraft_position.lat);
   filtered_position.lon = filter(filtered_position.lon,telemetry::m_aircraft_position.lon);
   filtered_position.alt = filter(filtered_position.alt,telemetry::m_aircraft_position.alt);
}

void telemetry::recalc()
{
  
   quan::angle_<double>::deg  dlat = filtered_position.lat - m_home_position.lat; // sort diffence for angle
   quan::angle_<double>::deg  dlon = filtered_position.lon - m_home_position.lon;

  // static_assert(std::is_same<decltype(dlat),quan::angle::deg>::value , "odd");
   quan::length_<double>::m  rlat = cos(quan::angle_<double>::rad{abs(m_home_position.lat)}) * radius_of_world();
   
   quan::length_<double>::m  dx = (dlon * rlat) / quan::angle_<double>::rad{1};
 //  quan::length_<double>::m  dx = quan::angle_<double>::rad{dlon} * rlat;
   //quan::length_<double>::m  dy = quan::angle_<double>::rad{dlat} * radius_of_world;
   quan::length_<double>::m dy =  (dlat * radius_of_world() ) / quan::angle_<double>::rad{1};
   quan::length_<double>::m  distance = quan::sqrt(dx * dx + dy * dy);
   quan::angle_<double>::deg elevation{90};
   quan::angle_<double>::deg bearing{0};

   if ( abs(distance) > quan::length::m{2}){
      bearing = quan::atan2(-dy, dx) + quan::angle::pi/2;
       // bearing = quan::atan2(dx,dy);
      elevation = quan::atan2(filtered_position.alt - m_home_position.alt, distance);
   }

   azimuth::motor::set_azimuth(bearing);
   main_loop::set_elevation(elevation);

#ifdef DEBUG
   static int count = 0;
   if ( ++ count == 5){
      count = 0;
      char buf[100];
      sprintf(buf,"distance = %f m\n",static_cast<double>(distance.numeric_value()));
      debug::serial_port::write( buf);
      sprintf(buf,"bearing = %f deg\n",static_cast<double>(bearing.numeric_value()));
      debug::serial_port::write( buf);
      sprintf(buf,"elevation = %f deg\n",static_cast<double>(elevation.numeric_value()));
      debug::serial_port::write( buf);
   }
#endif
   
}
namespace {

   unsigned char telem_message [7];
//   constexpr auto filter_k = 0.75f;
//
//   template <typename T>
//   T filter (T const & value, T const & new_value){
//      return value * filter_k + new_value * ( 1 - filter_k);
//   }

   void eval_telem_message()
   {
      if (quan::uav::do_checksum(telem_message) != telem_message[6]){
         return;
      }
      int32_t rawdata = (int32_t)(
         static_cast<uint32_t>( telem_message[2])
         + (static_cast<uint32_t>(telem_message[3]) << 8U) 
         + (static_cast<uint32_t>(telem_message[4]) << 16U) 
         + (static_cast<uint32_t>(telem_message[5]) << 24U)
      );

      switch (telem_message[1]) {
         case quan::uav::frsky::msgID::GPSLAT: {
            quan::angle::deg lat = quan::angle::deg10e7{rawdata};
            telemetry::m_aircraft_position.lat = lat;
           
         }
         break;
         case quan::uav::frsky::msgID::GPSLON: {
            quan::angle::deg lon = quan::angle::deg10e7{rawdata};
            telemetry::m_aircraft_position.lon = lon;
         }
         break;
         case quan::uav::frsky::msgID::BAROALT: {
            quan::length::mm alt = quan::length::m {rawdata};
            telemetry::m_aircraft_position.alt = alt;
            telemetry::state_changed = true;
         }
         break;

         default:
           break;
      }
   }

   quan::fifo<unsigned char,3> telem_input_buffer;

}// nmespace

void telemetry::parse_input()
{
   static uint32_t msg_idx = 0;
   unsigned char buf[9];
   if ( telemetry::serial_port.get_frame(buf) && ( buf[0] == FrSky2WSP::CommandID::UserData)){
      uint32_t num_bytes = buf[1];
      if (( num_bytes == 0) || ( num_bytes > 6 )) {
         // invalid number of data so assume corrupted and resync
        msg_idx = 0;
        return;
      }else{
         for ( uint32_t i = 0; i < num_bytes; ++i){
            telem_input_buffer.put(buf[3+i]);
            if (telem_input_buffer.num_in_buffer() > 2){
               telem_input_buffer.get(telem_message[msg_idx]);
               if(  telem_message[msg_idx] == quan::uav::frsky::msgID::header_value){
                  if ( msg_idx != 0){  // header in data so restart
                     telem_message[0] = quan::uav::frsky::msgID::header_value;
                  }
                  msg_idx = 1;
               }else{
                  if ( msg_idx != 0){
                     if ( telem_message[msg_idx] == quan::uav::frsky::msgID::escape_value){
                        telem_input_buffer.get( telem_message[msg_idx]) ;
                        telem_message[msg_idx] ^= 0x60;
                     }
                     if ( ++msg_idx == 7){
                        msg_idx = 0;
                        eval_telem_message();
                     }
                  }
               }
            }
         }
      }
   }
}


