#ifndef QUANTRACKER_MAIN_LOOP_HPP_INCLUDED
#define QUANTRACKER_MAIN_LOOP_HPP_INCLUDED

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

#include <quan/time.hpp>
#include <quan/angle.hpp>
#include <quan/frequency.hpp>
#include <quan/meta/integer_max.hpp>

#include <quan/stm32/detail/get_bus.hpp>
#include <quan/stm32/sys_freq.hpp>

#include "resources.hpp"

extern "C" void setup();
extern "C" void TIM2_IRQHandler();

struct main_loop{

   typedef main_loop_timer timer;
 //  static bool new_iteration;
 //  static bool toggle_led;

   static constexpr quan::frequency::Hz bus_frequency{
      quan::stm32::get_bus_frequency<
         quan::stm32::detail::get_bus<main_loop::timer
      >::type>() 
   };
   static constexpr quan::time::ms period{20};
     // add a tiny bit for rounding down into the integer value
   static constexpr uint32_t sys_clk_count = period * bus_frequency + 0.01;
   static_assert(sys_clk_count == 3360000 /4 ,"oops calc error");
   // arbitrary  period count
   // at period of  1/50th sec gives a resolution of 0.5 us
   static constexpr uint32_t max_count = 40000;
   // so what pre-scaler is needed?
   static constexpr uint32_t prescaler = ((2 *sys_clk_count) / max_count) - 1;
   static_assert( (sys_clk_count % max_count) == 0 ,"calc error");
   static_assert(prescaler < quan::meta::integer_max<uint16_t>::value,"calc error");
   static_assert(prescaler == 41,"oops calc error");

   static void set_elevation_servo(quan::time_<uint32_t>::us pos)
   {
       elevation_servo_pulse_len = pos;  
   }

   static void set_elevation(quan::angle::deg);

   static quan::time_<uint32_t>::us get_elevation_servo()
   {
       return  elevation_servo_pulse_len;
   }
private:
   static void setup();
   static void elevation_servo_setup_pwm();
   static void elevation_servo_setup_ports();
   friend void ::setup();
   friend void ::TIM2_IRQHandler();
   static quan::time_<uint32_t>::us elevation_servo_pulse_len;
};



#endif // QUANTRACKER_MAIN_LOOP_HPP_INCLUDED
