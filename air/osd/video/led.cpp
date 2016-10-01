/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korhonen(Openpilot.org)
   taulabs ( taulabs.com) 
   brainFPV ( brainfpv.com)
   Thomas Oldbury (super-osd.com)

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

#include "../resources.hpp"

namespace {
      
   template <typename Pin>
   void do_led_pin_setup()
   {
      quan::stm32::module_enable<typename Pin::port_type>();

      quan::stm32::apply<
         Pin
         , quan::stm32::gpio::mode::output
         , quan::stm32::gpio::otype::push_pull
         , quan::stm32::gpio::pupd::none
         , quan::stm32::gpio::ospeed::slow
         , quan::stm32::gpio::ostate::low
      >();
   }
}

namespace detail{
   void setup_leds()
   {
   #if (QUAN_OSD_BOARD_TYPE == 4) && !(defined QUAN_DISCOVERY)
      do_led_pin_setup<heartbeat_led_pin>();
       #if defined QUAN_AERFLITE_BOARD
         do_led_pin_setup<notify_led1>();
         do_led_pin_setup<notify_led2>();
       #endif
   #else
     do_led_pin_setup<heartbeat_led_pin>();
     do_led_pin_setup<blue_led_pin>();     
     do_led_pin_setup<green_led_pin>();  
     do_led_pin_setup<orange_led_pin>();
   #endif
   }
}