
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
#include <task.h>
#include <queue.h>
#include <quan/stm32/millis.hpp>
#include <quan/uav/osd/api.hpp>
#include "system/resources.hpp"
#include "azimuth/encoder.hpp"
#include "azimuth/servo.hpp"

#include "osd/on_telemetry_received.hpp"

#if defined QUANTRACKER_GROUND_COMMANDLINE_MODE
void parse_commandline();
#else
void tracking_update(quan::uav::osd::norm_position_type const & pos);
#endif

QueueHandle_t get_modem_telem_queue_handle();

bool button_pressed(); // atomic
void clear_button_pressed(); // atomic

namespace {
   QUAN_QUANTITY_LITERAL(time,ms)
   
   quan::uav::osd::norm_position_type next_pos;
   auto constexpr max_data_delay = 250_ms;
   bool data_good = true;
   quan::time_<int64_t>::ms last_data_time {0LL};

   void tracker_task(void * params)
   {
      gcs_serial::print<100>("starting tracker task at %lu\n", static_cast<uint32_t>(quan::stm32::millis().numeric_value()));

      quan::stm32::set<blue_led_pin>();
      quan::stm32::clear<green_led_pin>();
      quan::stm32::clear<heartbeat_led_pin>();

      for(;;){ 
      #if defined QUANTRACKER_GROUND_COMMANDLINE_MODE
         parse_commandline();
      #else
         auto const now = quan::stm32::millis();
         
         bool const new_data  =   (
            ( xQueueReceive(get_vrx_telem_queue_handle(),&next_pos,0) == pdTRUE) || 
            ( xQueueReceive(get_modem_telem_queue_handle(),&next_pos,0) == pdTRUE) 
         );

         if (new_data) {
            data_good = true;
            tracking_update(next_pos);
            quan::stm32::clear<blue_led_pin>();
            quan::stm32::set<green_led_pin>();
            last_data_time = now;
            
         }else {
            
            if ( data_good && (( now - last_data_time) >= max_data_delay)){
               // really want to beep here
               data_good = false;
               quan::stm32::set<blue_led_pin>();
               quan::stm32::clear<green_led_pin>();
            }
         }
         vTaskDelay(5);
      #endif
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
   constexpr uint32_t task_stack_size = 2000U;
   StackType_t __attribute__ ((used,section (".task_stacks"))) task_stack[task_stack_size];
   StaticTask_t __attribute__ ((used,section (".task_stacks"))) task_buffer;
}

void create_tracker_task()
{
   task_handle = ::xTaskCreateStatic(
      tracker_task
      ,"tracker_task"
      ,task_stack_size
      ,&dummy_param
      ,local_task_priority::tracker_task
      ,task_stack
      ,&task_buffer
   );
}
