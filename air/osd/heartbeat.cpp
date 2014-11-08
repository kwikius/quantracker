/*
 Copyright (c) 2003-2014 Andy Little.

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
#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#include <quan/stm32/gpio.hpp>

#include "heartbeat.hpp"
#include "resources.hpp"

/*
  On each heartbeat from mavlink
   switch on heartbeat led for 250 ms
*/

namespace {

   SemaphoreHandle_t heartbeat_semaphore = 0;

   void on_new_heartbeat()
   {
      xSemaphoreTake(heartbeat_semaphore,portMAX_DELAY);
   }

   void heartbeat_task(void * params)
   {
      heartbeat_semaphore = xSemaphoreCreateBinary();

      for (;;){
         on_new_heartbeat(); // blocks waiting for heartbeat
         quan::stm32::set<heartbeat_led_pin>(); 
         vTaskDelay(250);
         quan::stm32::clear<heartbeat_led_pin>(); 
      }
   }
}//namespace 
// N.B. called by mavlink task
void signal_new_heartbeat()
{
   xSemaphoreGive(heartbeat_semaphore);
}

void create_heartbeat_task()
{
   quan::stm32::module_enable<heartbeat_led_pin::port_type>();

   quan::stm32::apply<
      heartbeat_led_pin
      , quan::stm32::gpio::mode::output
      , quan::stm32::gpio::otype::push_pull
      , quan::stm32::gpio::pupd::none
      , quan::stm32::gpio::ospeed::slow
      , quan::stm32::gpio::ostate::low
   >();
   char dummy_param = 0;
   xTaskCreate(heartbeat_task,"heartbeat_task", 
      configMINIMAL_STACK_SIZE,
         &dummy_param,task_priority::heartbeat,
         ( TaskHandle_t * ) NULL);

}
