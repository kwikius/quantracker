
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

#include <stm32f4xx.h>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/sys_freq.hpp>
#include <quan/meta/integer_max.hpp>
#include <quan/frequency.hpp>
#include "main_loop.hpp"
#include "azimuth.hpp"
#include <quan/min.hpp>
#include <quan/max.hpp>
#include "events.hpp"


//bool main_loop::new_iteration = false;
//bool main_loop::toggle_led = false;
quan::time_<uint32_t>::us main_loop::elevation_servo_pulse_len{1500U} ;

void main_loop::set_elevation(quan::angle::deg angle_in)
{  
   auto angle = quan::max(quan::min(angle_in, quan::angle::deg{90}),quan::angle::deg{0});
   auto const pulse_90 = quan::time_<uint32_t>::us{1900U};
   auto const pulse_0 = quan::time_<uint32_t>::us{1000U};
   auto pulse_time = pulse_0 + (pulse_90 - pulse_0) * angle / quan::angle::deg{90};
   set_elevation_servo(pulse_time);
}

using namespace quan::stm32;

void main_loop::setup()
{
   disable<timer>();
   module_enable<timer>();
   
   timer::get()->psc = static_cast<uint16_t>(prescaler);
   timer::get()->arr = max_count; //
   // set cnt so that update event happens immediately
   timer::get()->cnt = max_count-1;

   auto cr1_reg = timer::get()->cr1.get();
   typedef decltype(cr1_reg) cr1_vt;

      // no clock division
      cr1_reg &= ~(quan::bit<cr1_vt>(9) | quan::bit<cr1_vt>(8) ); //(CKD[1:0])
      // preload the auto reload cr1_reg permanently to give a constant period
      // now the ARR has been written above
      cr1_reg &= ~quan::bit<cr1_vt>(7) ; // (ARPE)
      // set edge aligned counting
      cr1_reg &= ~(quan::bit<cr1_vt>(6) | quan::bit<cr1_vt>(5)); //(CMS[1:0])
      // set counting direction up
      cr1_reg &= ~quan::bit<cr1_vt>(4); // (DIR)
      // Not one pulse mode
      cr1_reg &= ~quan::bit<cr1_vt>(3); // (OPM)

      //only counter overflow generates an update irq or DMA request if enebled
      cr1_reg |= quan::bit<cr1_vt>(2); // (URS)
      // disable update event
      // NOTE: may want update event enabled to load new value in CC cr1_reg
      cr1_reg |= quan::bit<cr1_vt>(1); // (UDIS)
      
   timer::get()->cr1 = cr1_reg;

   // all interrupts and DMA disabled
   // will be enabled in subrouitnes
   timer::get()->dier = 0;
   // no events generated
   timer::get()->egr = 0;
   //  slave mode disabled - timer counts using internal clock
   // (other flags in smcr all relate to other modes, so dont care)
   timer::get()->smcr &= ~static_cast<uint32_t>(0b111);// ( SMS[2:0])

   azimuth::motor::setup_calc_compare();
   azimuth::motor::setup_pwm();
   main_loop::elevation_servo_setup_pwm();
    
   // enable the interrupt which should do for both interrupts and priority
   NVIC_SetPriority(TIM2_IRQn, interrupt_priority::loop_timer);
   static_assert(std::is_same<timer,tim2>::value , "wrong timer irqn for set irq prio");
   NVIC_EnableIRQ(TIM2_IRQn);

   //enable update interrupt
   timer::get()->dier.setbit<0>(); // (UIE)
   timer::get()->cr1.clearbit<1>(); // (UDIS)
 
   // go
   enable<timer>();
}

#define LOOP_TIMER_IRQHANDLER TIM2_IRQHandler
extern "C" void LOOP_TIMER_IRQHANDLER() __attribute__ ((interrupt ("IRQ")));
extern "C" void LOOP_TIMER_IRQHANDLER() 
{
   static_assert(std::is_same< main_loop::timer,tim2>::value , "wrong timer irq for handler");

   auto irq_flags = main_loop::timer::get()->sr.get();
   typedef decltype(irq_flags) flags_vt;

   // timer overflow interrupt
   if( irq_flags & quan::bit<flags_vt>(0) ){// (UIF)
     
      //load the compare register to validate
      main_loop::timer::get()->ccr4 = azimuth::motor::period_before_calc;
      main_loop::timer::get()->ccr2 = main_loop::elevation_servo_pulse_len.numeric_value() * 2;
      main_loop::timer::get()->sr.bb_clearbit<0>(); // (UIF)
      ms20_event.set();
   }
   // compare interrupt for calc prior to overflow
   if ( irq_flags & quan::bit<flags_vt>(4)){
      azimuth_pwm_calc_event.set();
      main_loop::timer::get()->sr.bb_clearbit<4>(); // (CC4IF)
   }
}
#undef LOOP_TIMER_IRQHANDLER
   
