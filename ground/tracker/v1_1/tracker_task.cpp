
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

bool button_pressed(); // atomic
void clear_button_pressed(); // atomic

namespace {
   quan::uav::osd::norm_position_type next_pos;
   constexpr TickType_t vrx_telem_wait_time_ms = 50U;

   void tracker_task(void * params)
   {
      auto const now = quan::stm32::millis();
      gcs_serial::print<100>("starting tracker task at %lu\n", static_cast<uint32_t>(now.numeric_value()));
      for(;;){ 
      #if defined QUANTRACKER_GROUND_COMMANDLINE_MODE
         parse_commandline();
      #else
         if ( xQueueReceive(get_vrx_telem_queue_handle(),&next_pos,vrx_telem_wait_time_ms) == pdTRUE){
            tracking_update(next_pos);
            quan::stm32::clear<blue_led_pin>();
         }else{
            quan::stm32::set<blue_led_pin>();
         }
      #endif
       // TODO add receive from rf modem
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
   constexpr uint32_t task_stack_size = 3000U;
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
