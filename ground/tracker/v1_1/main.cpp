
/*
 Copyright (c) 2013 -2017 Andy Little 

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

#include <stm32f4xx.h>
#include <FreeRTOS.h>
#include <task.h>
#include "system/osd_resources.hpp"

extern "C" void setup();
bool initialise_flash();
void signal_exit_failure();
void create_draw_task();
void create_telemetry_receiver_task();
void create_tracker_task();

int main()
{
   if (! initialise_flash()){
      signal_exit_failure();
   }

   setup();
   create_telemetry_receiver_task();
   create_draw_task();
   create_tracker_task();

   vTaskStartScheduler();

   while (1) {;}
}

void signal_exit_failure()
{
   quan::stm32::module_enable< heartbeat_led_pin::port_type>();
   quan::stm32::apply<
      heartbeat_led_pin
      , quan::stm32::gpio::mode::output
      , quan::stm32::gpio::otype::push_pull
      , quan::stm32::gpio::pupd::none
      , quan::stm32::gpio::ospeed::slow
      , quan::stm32::gpio::ostate::low
   >();
   for(;;){asm volatile ("nop":::);}
}
