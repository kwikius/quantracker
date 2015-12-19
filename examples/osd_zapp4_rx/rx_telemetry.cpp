

#include "resources.hpp"
#include "rx_telemetry.hpp"
#include <quan/tracker/zapp4/position.hpp>
#include <quan/tracker/zapp3_encode.hpp>
#include <quan/uav/cobs/packet_parser.hpp>

void parse_data(char ch);

namespace {
  quan::uav::cobs::packet_parser* packet_parser = nullptr;
//  uint32_t good_packets_received = 0;
//  uint32_t packets_received = 0;
  
}

//uint32_t get_good_packets_received()
//{
//  return good_packets_received;
//}
//
//uint32_t get_packets_received()
//{
//  return packets_received;
//}

void setup_telemetry_parser()
{
   packet_parser = new quan::uav::cobs::packet_parser{19}; // set to the size of the largest encoded packet
   quan::stm32::set<heartbeat_led_pin>();
}

void on_telemetry_received()
{
   char buf[200];
   size_t const data_len = get_telemetry_receiver_buffer_length();
   read_telemetry_data(buf,data_len);
   for (size_t i = 0; i < data_len; ++i){
      parse_data(buf[i]);
   }
}

namespace {

   void do_output( quan::uav::osd::norm_position_type const & pos)
   {  
     if ( mavlink_tx_rx_task::out_space_avail() >= 16){
         uint8_t encoded[16];
         quan::tracker::zapp3_encode (pos,encoded);
         mavlink_tx_rx_task::write(encoded,16);
     } 
   }

    uint32_t count = 0;
}

void parse_data(char ch)
{
   uint16_t const packet_length = packet_parser->parse(ch);
   if ( packet_length > 0 ){

      // got a packet
      //++ packets_received;
      uint8_t const * decoded_packet_buffer = packet_parser->get_decoded_packet();
      // check its length matches the command
      uint16_t command_id = decoded_packet_buffer[0];
      switch(command_id){
         case quan::tracker::zapp4::command_id::position:{
               if (packet_length == quan::tracker::zapp4::get_decoded_packet_size(command_id)){
                  quan::uav::osd::norm_position_type pos;
                  bool const result = quan::tracker::zapp4::get_position(packet_parser->get_decoded_packet(),pos);
                  if (result){
                     quan::uav::osd::norm_position_type * pos_var = mutex_acquire_position(5);
                     if(pos_var){
                        *pos_var = pos;
                        mutex_release_position();
                           if (++count == 50){
                              count = 0;
                              quan::stm32::complement<heartbeat_led_pin>();
                           }
                     }
                  }
               }
            }
            break;
         default:{
            break;
         }
      }
   }
}
