/*
 Copyright (c) 2003-2015 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */

#include <FreeRTOS.h>
#include <task.h>
#include <quan/tracker/zapp3_encode.hpp>
#include "fsk.hpp"
#include "resources.hpp"
#include "aircraft.hpp"

namespace {

   typedef quan::uav::position<
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::mm  // N.B changed to mm from old protocol
   > pos_type;

} //namespace

namespace fsk {

   void send_message()
   {
    asm volatile ("nop":::);

      the_aircraft.mutex_acquire();
         pos_type pos = the_aircraft.location;
      the_aircraft.mutex_release();
      
      uint8_t encoded[16];
      quan::tracker::zapp3_encode (pos,encoded);

      fsk::write((const char*)encoded, 16);
   }

   void fsk_task(void* params)
   {
       TickType_t last_wakeup = xTaskGetTickCount();
       for (;;){
         vTaskDelayUntil(&last_wakeup,200);
         fsk::send_message();
      }
   }
}

namespace {
   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
}

void create_fsk_task()
{
   xTaskCreate(fsk::fsk_task,
      "fsk_task", 
      256,
      &dummy_param,
      task_priority::fsk,
      &task_handle);
}
 
