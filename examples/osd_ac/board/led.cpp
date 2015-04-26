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

#if (QUAN_OSD_BOARD_TYPE != 4) || (defined QUAN_DISCOVERY)
#include <quan/meta/type_sequence.hpp>
#include <quan/meta/for_each.hpp>
#endif

#include "resources.hpp"

namespace {
      
   struct do_led_pin_setup
   {
      template <typename Pin>
      void operator()()const
      {
         quan::stm32::module_enable< typename Pin::port_type>();

         quan::stm32::apply<
            Pin
            , quan::stm32::gpio::mode::output
            , quan::stm32::gpio::otype::push_pull
            , quan::stm32::gpio::pupd::none
            , quan::stm32::gpio::ospeed::slow
            , quan::stm32::gpio::ostate::low
         >();
      }
   };
}
#if (QUAN_OSD_BOARD_TYPE != 4) || (defined QUAN_DISCOVERY)
namespace  quan{ namespace impl{
   template<> struct is_model_of_impl<
      quan::meta::PolymorphicFunctor<1,0>,do_led_pin_setup 
   > : quan::meta::true_{};
}}
#endif

void setup_leds()
{
#if (QUAN_OSD_BOARD_TYPE == 4) && !(defined QUAN_DISCOVERY)
      do_led_pin_setup{}.operator()<heartbeat_led_pin>();
#else
   typedef quan::meta::type_sequence<
      heartbeat_led_pin
      ,blue_led_pin     
      ,green_led_pin  
      ,orange_led_pin 
   > led_pins;
   quan::meta::for_each<led_pins,do_led_pin_setup>{}();
 #endif
}