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
#include "FreeRTOS.h"
#include "task.h"
#include "resources.hpp"

#include "frsky.hpp"

namespace frsky{

   void send_message()
   {
      zapp1::frsky_send_message();
   }

   void frsky_task(void * param)
   {
      for (;;){
         frsky::send_message();
         vTaskDelay(50);
      }
   }
}//frsky

void create_frsky_task()
{
   char dummy_param = 0;
   xTaskCreate(frsky::frsky_task,"frsky_task", 
      configMINIMAL_STACK_SIZE,
         &dummy_param,task_priority::frsky,
         ( TaskHandle_t * ) NULL);
}
