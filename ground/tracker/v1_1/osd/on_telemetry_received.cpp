
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

#include "FreeRTOS.h"
#include <semphr.h>
#include <quan/uav/position.hpp>
#include <quan/uav/osd/api.hpp>
#include <quan/time.hpp>
#include <quantracker/osd/telemetry_receiver.hpp>
#include "../system/osd_resources.hpp"
#include "../system/tracker_resources.hpp"
#include <quan/stm32/gpio.hpp>
#include <quan/tracker/zapp4/position.hpp>
#include <quan/uav/cobs/packet_parser.hpp>

#include "on_telemetry_received.hpp"

namespace {

  quan::uav::cobs::packet_parser* packet_parser = nullptr;
  uint32_t m_num_errors = 0;

  QueueHandle_t vrx_telem_queue_handle = nullptr;

  void parse_data(char ch);

}

QueueHandle_t get_vrx_telem_queue_handle()
{
   return vrx_telem_queue_handle;
}

uint32_t get_num_telem_errors()
{
   return m_num_errors;
}

void clear_telem_errors()
{
   m_num_errors = 0;
}

/*
  call in setup
*/
void setup_telemetry_parser()
{
   packet_parser = new quan::uav::cobs::packet_parser{19}; // set to the size of the largest encoded packet
   vrx_telem_queue_handle = xQueueCreate(1,sizeof(quan::uav::osd::norm_position_type));
}

// callback
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

   void inc_errors()
   {
       vTaskSuspendAll();
       quan::stm32::set<heartbeat_led_pin>();
       quan::stm32::clear<green_led_pin>();
       xTaskResumeAll();
       ++m_num_errors;
   }

/*
    set green led on good and red led on bad data
*/

   void parse_data(char ch)
   {
      uint16_t const packet_length = packet_parser->parse(ch);
      m_num_errors += packet_parser->clear_errors();
      if ( packet_length > 0 ){
         uint8_t const * decoded_packet_buffer = packet_parser->get_decoded_packet();
         // check its length matches the command
         uint16_t command_id = decoded_packet_buffer[0];
         switch(command_id){
            case quan::tracker::zapp4::command_id::position:{
                  if (packet_length == quan::tracker::zapp4::get_decoded_packet_size(command_id)){
                     quan::uav::osd::norm_position_type pos;
                     bool const result = quan::tracker::zapp4::get_position(packet_parser->get_decoded_packet(),pos);
                     if (result && (get_vrx_telem_queue_handle() != nullptr)){
                         vTaskSuspendAll();
                         quan::stm32::set<green_led_pin>();
                         quan::stm32::clear<heartbeat_led_pin>();
                         xTaskResumeAll();
                         xQueueOverwrite(get_vrx_telem_queue_handle(),&pos);
                     }else{
                        inc_errors();
                     }
                  }else{
                     inc_errors();
                  }
               }
               break;
            default:{
                inc_errors();
               break;
            }
         }
      }
   }
}
