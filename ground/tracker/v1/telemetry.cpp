
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

//#include <quan_matters/src/uav/cobs/protocol.cpp>

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

}// namespace

namespace {
   
   // decoded cobs telem message
   // what each byte reprsents is dependent on ar[0];
   // see the eval_cobs_telem_message fun
   uint8_t cobs_telem_message [14];

   // array of current position marshalled from input
   // this  array represents the current position state
   // on  telemetry input some or all bytes are be updated
   // the whole array is then assumed valid
   // and is used to reinitialise the aircraft position info
   uint8_t cur_position_data[11] = {0,0,0,0,0,0,0,0,0,0,0};

   // convert cur_position_data input to the current aircraft position
   // in telemetry::m_aircraft_position
   void update_position_from_array_input()
   {
      int32_t const raw_lat 
      = (int32_t)(static_cast<uint32_t>(cur_position_data[0]) 
         |  (static_cast<uint32_t>(cur_position_data[1]) << 8U)
         |  (static_cast<uint32_t>(cur_position_data[2]) << 16U)
         |  (static_cast<uint32_t>(cur_position_data[3]) << 24U)
      );

      int32_t const raw_lon 
      = (int32_t)(static_cast<uint32_t>(cur_position_data[4]) 
         |  (static_cast<uint32_t>(cur_position_data[5]) << 8U)
         |  (static_cast<uint32_t>(cur_position_data[6]) << 16U)
         |  (static_cast<uint32_t>(cur_position_data[7]) << 24U)
      );

      // this one comes in as a int24_t
      uint32_t raw_alt_pre
      = static_cast<uint32_t>(cur_position_data[8]) 
         |  (static_cast<uint32_t>(cur_position_data[9]) << 8U)
         |  (static_cast<uint32_t>(cur_position_data[10]) << 16U)
      ;
      //  sort sign
      if ( raw_alt_pre & 0x800000){
         raw_alt_pre |= 0xFF000000;
      }
      // finally reinterpret as int32_t
      int32_t const raw_alt = (int32_t) raw_alt_pre;

      quan::angle::deg const lat = quan::angle::deg10e7{raw_lat};
      telemetry::m_aircraft_position.lat = lat;
           
      quan::angle::deg const lon = quan::angle::deg10e7{raw_lon};
      telemetry::m_aircraft_position.lon = lon;

      quan::length::mm const alt = quan::length::m {raw_alt};
      telemetry::m_aircraft_position.alt = alt;

      telemetry::state_changed = true;
   }

   // The message contains info to update the whole position data
   bool eval_cobs_full_telem_message()
   {
      //(msg_len has been checked as 14)
      // has been checked that arr[0] == 8U
      static constexpr uint8_t msg_len = 14;
      uint16_t const ck1 = quan::uav::fletcher16(cobs_telem_message,msg_len-2);
      uint16_t const ck2 = static_cast<uint16_t>(cobs_telem_message[msg_len-2]) | (static_cast<uint16_t>( cobs_telem_message[msg_len-1]) << 8);
      if ( ck1 != ck2){
         return false; // bad checksum
      }
      memcpy(cur_position_data,cobs_telem_message+1,11);
      update_position_from_array_input();
      return true;
   }

   bool eval_cobs_telem_message(uint8_t msg_len)
   {
      // msglen should be 4,6,7,9,10,12,13,14
    
      uint8_t const numdata = cobs_telem_message[0] & 0xF;
      switch (msg_len){
         // numdata is a function of length
         // so check its correct for the length
         case 4: 
            if ( numdata != 1) {
               return false;
            } else {
               break;
            }
         case 6:
            if ( numdata != 2) {
               return false;;
            } else {
               break;
            }
         case 7:
            if ( numdata != 3) {
               return false;
            } else {
               break;
            }
         case 9:
            if ( numdata != 4) {
               return false;
            } else {
               break;
            }
         case 10:
            if ( numdata != 5) {
               return false;
            } else {
               break;
            }
         case 12:
            if ( numdata != 6) {
               return false;
            } else {
               break;
            }
         case 13:
            if ( numdata != 7) {
               return false;
            } else {
               break;
            }
         case 14: // The length of the full message
            if(cobs_telem_message[0] == 8U){
               return eval_cobs_full_telem_message();
            }else{
               return false; // invalid length id in data so corrupt packet
            }
         default:
            return false; // not a valid message length
      }
      // partial message only evaled here
      // do checksum
      uint16_t const ck1 = quan::uav::fletcher16(cobs_telem_message,msg_len-2);
      uint16_t const ck2 = static_cast<uint16_t>(cobs_telem_message[msg_len-2]) | (static_cast<uint16_t>( cobs_telem_message[msg_len-1]) << 8);
      if ( ck1 != ck2){
         return false; // bad checksum
      }
      
      // make local copy of the cur_position_data array
      // (we will only update the cur array from here if no errors are found)
      uint8_t temp_position_data[11];
      memcpy(temp_position_data,cur_position_data,11);
   
      // The offset of the start of the data in the input
      // (The indices of each byte are encoded in the nibbles in the first bytes)
      uint8_t position_data_offset = numdata / 2 + 1;
      // The last position data index extracted from the input
      // update as we go thru loop
      int16_t last_position_data_array_index = -1;

      // Find the index of each byte of data and update the local array in that position
      for ( uint8_t i = 0; i < numdata; ++i){
         // index of the position_data index in the input
         // the indices are nibbles
         uint8_t data_index_index = (i + 1) / 2;
         // dependent on whether the next index is odd or even its in low or high nibble
         uint8_t position_data_array_index = (( i & 1 ) == 0)
            ? static_cast<uint8_t>((cobs_telem_message[data_index_index] & 0xF0) >> 4 )
            : static_cast<uint8_t>(cobs_telem_message[data_index_index] & 0xF);
         // Sanity check. Each position_data_index must be higher than the previous and in range 0 to 10
         if ( (static_cast<int16_t>(position_data_array_index) <= last_position_data_array_index) || ( position_data_array_index > 10U) ){
            // data corrupted
            return false;
         }else{
            // put the byte in the local array at its index
            temp_position_data[position_data_array_index] = cobs_telem_message[position_data_offset++];
            // and update the state
            last_position_data_array_index = position_data_array_index;
            continue;
         }
      }
      // Success! .. copy data back to gps structure
      memcpy(cur_position_data,temp_position_data,11);
     // and convert back into aircraft position
      update_position_from_array_input();
      return true;
   }

}// namespace


// use a hardware switch or eeprom config to decide which
// protocol to use?
void telemetry::parse_input()
{
   switch (get_protocol()){
   case protocol_t::command_line:
      parse_commandline();
      break;
   case protocol_t::zapp2:
      parse_input_from_FrSky_with_cobs_protocol();
      break;
   default:
      parse_input_from_FrSky_with_high_level_escape_protocol();
      break;
   }
 //  parse_input_from_FrSky_with_cobs_protocol();
}

 // FrSky telemetry from RcTx with cobbs protocol on top

void telemetry::parse_input_from_FrSky_with_cobs_protocol()
{
  // buffer for the raw input message in FrSky protocol format
   unsigned char frsky_buf[9];

   // max length of encoded cobs message not including the framing zero
   static const int max_cobs_len = 15;
   static unsigned char cobs_buf[max_cobs_len];
   static uint32_t cobs_idx = 0;
   static bool cobs_synced = false;

    // Get an Frsky frame from FrSky telemetry receiver and 
    // process it if it is a User Data frame,
    // else ignore it.
    // if get_frame returns true , a new packet has been put in buf
    // Buf[0] is the Frsky ID for the data packet
    // If the frSky ID signifies user data
    // then buf[1] is the number of bytes
    // buf[2] is empty
    // buf[3] onwards are the data bytes 
    // N.B. The get_frame function has already removed the low level FrSky telemetry escapes.
   if ( telemetry::serial_port.get_frame(frsky_buf) 
         && ( frsky_buf[0] == FrSky2WSP::CommandID::UserData)){

      uint32_t const num_bytes = frsky_buf[1];
      // the number of bytes has been checked in sp.get_frame as being 1 to 6
      for ( uint32_t i = 0; i < num_bytes; ++i){
         uint8_t const data = frsky_buf[3+i];
         if ( cobs_synced){
            if ( cobs_idx <= max_cobs_len){
               if (data != 0){
                  if ( cobs_idx < max_cobs_len ){ // if data is non-zero cant be last byte
                     cobs_buf[cobs_idx++] = data;
                  }else{
                     cobs_synced = false;
                  }
               }else{ // valid end of frame zero byte
                   // if decode or eval fails then resync.
                   // could add another framing byte to deal with this?
                  if ( quan::uav::cobs::decode(cobs_buf,cobs_idx,cobs_telem_message)   
                        // (decoding subtracts 1 from length of message)
                        && eval_cobs_telem_message(cobs_idx -1)){
                        // success
                     cobs_idx = 0;
                  }else{
                     cobs_synced = false;
                  }
               }
            }else{ // message too long
               cobs_synced = false;
            }
         }else{ // not synced yet, look for zero byte
            if (data == 0){
               cobs_synced = true;
               cobs_idx = 0;
            }
         }
      }
   }
}

namespace{
   quan::fifo<unsigned char,3> two_byte_escape_buffer;
}

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


