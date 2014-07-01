#include <stdlib.h>
#include <stdio.h>
#include <quan/config.hpp>
#include "serial_ports.hpp"
#include "telemetry.hpp"
#include <quan/uav/msg_protocol.hpp>
#include <quan/uav/do_checksum.hpp>

// bytestuff telem input
namespace {

   unsigned char telem_message [7];
   
   void eval_telem_message()
   {
      if (quan::uav::do_checksum(telem_message) != telem_message[6]){
         return;
      }
      int32_t const rawdata = (int32_t)(
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

   quan::fifo<unsigned char,3> two_byte_escape_buffer;

}// namespace

// FrSky telemetry from RcTx with original high level escapes protocol on top
void telemetry::parse_input_from_FrSky_with_high_level_escape_protocol()
{
   // buffer for the raw input message in FrSky protocol format
   unsigned char buf[9];

   static  uint32_t telem_msg_idx = 0; 
   
    // Get an Frsky frame from FrSky telemetry receiver and process it if it is a User Data frame
    // else ignore it.
    // if get_frame returns true , a new packet has been put in buf
    // Buf[0] is the Frsky ID for the data packet
    // If the frSky ID signifies user data
    // then buf[1] is the number of bytes
    // buf[2] onwards are the data bytes 
    // N.B. The get_frame function has already removed the low level FrSky telemetry escapes.
   if ( telemetry::serial_port.get_frame(buf) && ( buf[0] == FrSky2WSP::CommandID::UserData)){

      uint32_t const num_bytes = buf[1];
      if (( num_bytes == 0) || ( num_bytes > 6 )) {
         // invalid number of data bytes so assume the message is corrupted and resync
        telem_msg_idx = 0;
        return;
      }else{
         // copy the user data we have into the telemetry input buffer
         // but first evaluate any higher levelly escaped data in the original escaped tracker telemetry protocol
         // we need to have at least 2 bytes available for processing the escapes
         for ( uint32_t i = 0; i < num_bytes; ++i){
            two_byte_escape_buffer.put(buf[3+i]);
            if (two_byte_escape_buffer.num_in_buffer() > 2){
               two_byte_escape_buffer.get(telem_message[telem_msg_idx]);
               // header should only appear at start of data
               if(  telem_message[telem_msg_idx] == quan::uav::frsky::msgID::header_value){
                  if ( telem_msg_idx != 0){  // header in data so restart
                     telem_message[0] = quan::uav::frsky::msgID::header_value;
                  }
                  telem_msg_idx = 1;  // will always now be at 1st data position
               }else{
                  if ( telem_msg_idx != 0){
                      // check for and evaluate escapes
                     if ( telem_message[telem_msg_idx] == quan::uav::frsky::msgID::escape_value){
                        two_byte_escape_buffer.get( telem_message[telem_msg_idx]) ;
                        telem_message[telem_msg_idx] ^= 0x60;
                     }
                     if ( ++telem_msg_idx == 7){ // all decoded high level messages are 7 bytes long
                        telem_msg_idx = 0;
                        eval_telem_message();
                     }
                  }
               }
            }
         }
      }
   }
}