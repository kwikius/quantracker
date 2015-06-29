
/*
 Copyright (c) 2013 -2015 Andy Little 

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

#include "resources.hpp"

extern "C" void setup();

void create_mavlink_task();
void create_heartbeat_task();
void create_frsky_task();
void create_fsk_task();
void create_draw_task();

#if ( QUAN_OSD_BOARD_TYPE !=4)
void create_leds_task();
#endif
void draw_loop();

void mode_check();
bool initialise_flash();

int main()
{
  //check if user wants to mod flash vars
  // also setss up flash on new firmware
  // without which flash cant be modified
  if (! initialise_flash()){
      // set heartbeat_led on permanently symbolise fail
       quan::stm32::module_enable< heartbeat_led_pin::port_type>();
         quan::stm32::apply<
            heartbeat_led_pin
            , quan::stm32::gpio::mode::output
            , quan::stm32::gpio::otype::push_pull
            , quan::stm32::gpio::pupd::none
            , quan::stm32::gpio::ospeed::slow
            , quan::stm32::gpio::ostate::high
         >();
      while (1){;}
  }

  mode_check();
  
  
  setup();

  //create_mavlink_task();
 // create_frsky_task();
  //create_heartbeat_task();
  //create_fsk_task();
  create_draw_task();
 // create_telem_task();
#if ( QUAN_OSD_BOARD_TYPE !=4)
  create_leds_task();
#endif
  vTaskStartScheduler();

  while (1) {;}
}
