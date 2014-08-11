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

/*
 
*/

#include "main_loop.hpp"

/*
  elev servo pwm is on loop_timer::cc2
*/
using namespace quan::stm32;

void main_loop::elevation_servo_setup_ports()
{
    module_enable<elev_servo_pwm_out_pin::port_type>();
    apply<
       elev_servo_pwm_out_pin
      ,gpio::mode::af1
      ,gpio::otype::push_pull
      ,gpio::pupd::none
      ,gpio::ospeed::slow
      ,gpio::ostate::low
   >();
}

// CC2 setup ----------------------------
void main_loop::elevation_servo_setup_pwm()
{
   // must be cleared to write ccmr2
   timer::get()->ccer &= ~quan::bit<uint32_t>(4); // (CC2E)
 
   timer::get()->ccr2 = elevation_servo_pulse_len.numeric_value() * 2;
   auto ccmr1 = timer::get()->ccmr1.get();
      // set CC2 to PWM mode upcounting active before match (OC2M = b110)
      ccmr1 |= (quan::bit<uint32_t>(14) | quan::bit<uint32_t>(13)); 
      ccmr1 &= ~quan::bit<uint32_t>(12); 
      // CCR2 selected as output
      ccmr1 &= ~(quan::bit<uint32_t>(9) | quan::bit<uint32_t>(8));  //(CC2S)
      //disable preload 
      ccmr1 &= ~quan::bit<uint32_t>(11) ; //(OC2PE)
      // output compare fast enebled is dont care I think
      ccmr1 &= ~quan::bit<uint32_t>(10);  //(OC2FE)
   timer::get()->ccmr1 = ccmr1;

   auto ccer = timer::get()->ccer.get();
      //  CC2NP  must be off if output
      ccer &= ~quan::bit<uint32_t>(7); //(CC2NP)
      // set output polarity active high
      ccer &= ~quan::bit<uint32_t>(5); //(CC2P) 
      // enable CC2 pin output
      ccer |= quan::bit<uint32_t>(4); // (CC2E)
   timer::get()->ccer = ccer;

}

