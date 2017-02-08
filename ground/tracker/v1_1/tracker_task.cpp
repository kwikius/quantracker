
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

#include "system/resources.hpp"
#include "azimuth/encoder.hpp"
#include "azimuth/servo.hpp"

void parse_commandline();

namespace {

   void tracker_task(void * params)
   {
      auto const now = quan::stm32::millis();
      gcs_serial::print<100>("starting tracker task at %lu\n", static_cast<uint32_t>(now.numeric_value()));
      for(;;){ 
         parse_commandline();
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
