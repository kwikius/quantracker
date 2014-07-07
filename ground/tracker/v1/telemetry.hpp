#ifndef TELEMETRY_HPP_INCLUDED
#define TELEMETRY_HPP_INCLUDED

#include <quan/uav/position.hpp>
#include "frsky_serial_port.hpp"

struct telemetry{

   typedef quan::angle::deg lat_lon_type;
   typedef quan::length::mm     altitude_type;

   typedef quan::uav::position<lat_lon_type,altitude_type> gps_position;
   static constexpr quan::length::m  			  radius_of_world(){ return quan::length::m{6371000};};

   static gps_position m_home_position;
   static gps_position m_aircraft_position;
   static frsky_serial_port serial_port;

   static void recalc();

   static void parse_input();
  
   static bool state_changed;
   static void set_home();
   static void filter_pos();
   enum class protocol_t {zapp1,zapp3,command_line};
   static void set_protocol (protocol_t prot){m_protocol = prot;}
   static protocol_t get_protocol(){return m_protocol;}
   private:
        static void parse_frsky_bytestuff();
        static void parse_av_cobs();
        static void parse_command_line();
        static protocol_t m_protocol;
   
};

void parse_commandline();


#endif // TELEMETRY_HPP_INCLUDED
