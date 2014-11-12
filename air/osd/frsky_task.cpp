/*
 Copyright (c) 2012 - 2013 Andy Little 

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
#include <FreeRTOS.h>
#include <task.h>

#include "resources.hpp"
#include "frsky.hpp"

namespace frsky{

   void send_message()
   {
      zapp1::frsky_send_message();
   }

   void frsky_task(void * param)
   {
      frsky_tx_rx_task::enable();
      TickType_t last_wakeup = xTaskGetTickCount();

      for (;;){
         vTaskDelayUntil(&last_wakeup,50);
         frsky::send_message();
      }
   }
}//frsky

namespace {
   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
}

void create_frsky_task()
{
   xTaskCreate(
      frsky::frsky_task,"frsky_task", 
      512,
      &dummy_param,
      task_priority::frsky,
      &task_handle
   );
}
