
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

#include "tracker_states.hpp"
#include "leds.hpp"
#include <tuple>
#include <quan/fun/for_each.hpp>

using namespace quan::stm32f4;

Led<green_led_pin>     tracking_led;
Led<orange_led_pin>       error_led;
Led<blue_led_pin>      startup_led;
Led<red_led_pin>    heartbeat_led;

namespace {

   // make a list of refs to leds for ease of use
   auto leds = std::make_tuple(
       std::ref(heartbeat_led)
      ,std::ref(tracking_led)
      ,std::ref(startup_led)
      ,std::ref(error_led)
   );
   
   struct setup
   {
      template <typename LedRef>
      void operator()(LedRef led)const
      {
         typedef typename decltype(led)::pin_type pin_type;

         module_enable<typename pin_type::port_type>();

         apply<
            pin_type
            , gpio::mode::output
            , gpio::otype::push_pull
            , gpio::pupd::none
            , gpio::ospeed::slow
            , gpio::ostate::low
         >();
      }
   };
   
    struct update{
      template <typename LedRef>
      void operator()(LedRef led)const
      {
         led.update();
      }
   };

} //~namespace

void setup_leds()
{
    quan::fun::for_each(leds,setup{});
}

void update_leds()
{
    quan::fun::for_each(leds,update{});
}
