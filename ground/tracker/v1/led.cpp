
/*
 Copyright (c) 2013 Andy Little 

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

#include <quan/meta/type_sequence.hpp>
#include <quan/concepts/meta/type_sequence/for_each.hpp>
#include "tracker_states.hpp"
#include "leds.hpp"

using namespace quan::stm32;


Led<green_led_pin>     tracking_led;
Led<orange_led_pin>       error_led;
Led<blue_led_pin>      startup_led;
Led<red_led_pin>    heartbeat_led;

namespace {
      
   struct do_led_pin_setup
   {
      template <typename Pin>
      void operator()()const
      {
         module_enable< typename Pin::port_type>();

         apply<
            Pin
            , gpio::mode::output
            , gpio::otype::push_pull
            , gpio::pupd::none
            , gpio::ospeed::slow
            , gpio::ostate::low
         >();
      }
   };
}

namespace  quan{ namespace impl{
   template<> struct is_model_of_impl<quan::meta::PolymorphicFunctor<1,0>,do_led_pin_setup > : quan::meta::true_{};
}}

void setup_leds()
{
   typedef quan::meta::type_sequence<
      red_led_pin
      ,blue_led_pin     
      ,green_led_pin  
      ,orange_led_pin 
   > led_pins;
  
   quan::meta::for_each<led_pins,do_led_pin_setup>{}();

   heartbeat_led.switch_off();
   tracking_led.switch_off();
   startup_led.switch_off();
   error_led.switch_off();
   
}

void update_leds()
{
   heartbeat_led.update();
   tracking_led.update();
   startup_led.update();
   error_led.update();
}
