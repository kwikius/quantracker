/*
 Copyright (c) 2014-2015 Andy Little.

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

#include "FreeRTOS.h"
#include "task.h"

#include "resources.hpp"

void request_osd_suspend();

void request_osd_resume();

bool osd_suspended();

namespace {
   void osd_suspend_task( void *param)
   {
        for(;;) {
           vTaskDelay(5000);
           request_osd_suspend();
           while (!osd_suspended()){;}
           vTaskDelay(1000);
           request_osd_resume();
           while (osd_suspended()){;}
        }
   }
   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
}

void create_osd_suspend_task( )
{
     xTaskCreate( osd_suspend_task, "osd_suspend",configMINIMAL_STACK_SIZE,
     &dummy_param,
      task_priority::heartbeat,
      &task_handle
      );
}

