
#include <stdlib.h>
#include <stdio.h>
#include "serial_ports.hpp"
#include <quan/config.hpp>
#include "telemetry.hpp"
#include "azimuth.hpp"
#include <quan/uav/msg_protocol.hpp>
#include <quan/uav/cobs/protocol.hpp>
#include "frsky_serial_port.hpp"
#include <quan/utility/fifo.hpp>
#include <quan/uav/do_checksum.hpp>
#include <quan/uav/fletcher16.hpp>
#include <quan/atan2.hpp>
#include <quan/uav/position.hpp>


telemetry::gps_position telemetry::m_home_position{quan::angle::deg{0},quan::angle::deg{0},quan::length::m{0}};
telemetry::gps_position  telemetry::m_aircraft_position{quan::angle::deg{0},quan::angle::deg{0},quan::length::m{0}};
frsky_serial_port telemetry::serial_port; 
bool telemetry::state_changed = false;
telemetry::protocol_t telemetry::m_protocol = telemetry::protocol_t::zapp1;

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

// either commandline or zapp3

void telemetry::parse_input()
{
   if(get_protocol() == protocol_t::command_line){
      parse_commandline();
   }else{
      parse_av_cobs();
      telemetry::parse_frsky_bytestuff();
   }
 //  parse_input_from_FrSky_with_cobs_protocol();
}

