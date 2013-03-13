
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

#include "azimuth.hpp"

bool azimuth::motor::m_enabled = false;
int32_t azimuth::motor:: m_target_position = 0;
float azimuth::motor::m_min_duty_cycle_percent = 0.1f;
int32_t azimuth::motor::m_last_recorded_position = 0;

float azimuth::motor::m_kP = 0.0045;
float azimuth::motor::m_kD = 0.01; // 0.001

using namespace quan::stm32f4;

void azimuth::motor::setup_ports()
{
   module_enable<pwm_pin::port_type>();
   module_enable<direction_pin::port_type>();
   module_enable<not_direction_pin::port_type>();

   apply<
      pwm_pin
      ,gpio::mode::af1     
      ,gpio::otype::push_pull  
      ,gpio::ospeed::slow 
      ,gpio::ostate::low
   >();

   apply<
      direction_pin
      ,gpio::mode::output
      ,gpio::otype::push_pull
      ,gpio::ospeed::slow
      ,gpio::ostate::low
   >();

   apply<
      not_direction_pin
      ,gpio::mode::output
      ,gpio::otype::push_pull
      ,gpio::ospeed::slow
      ,gpio::ostate::low
   >();
}

void azimuth::motor::setup_pwm()
{
   //disable interrupts and DMA from this channel
   main_loop::timer::get()->dier.clearbit<9>(); //(CC1DE) DMA 
   main_loop::timer::get()->dier.clearbit<1>(); //(CC1IE) Interrupt 

  //clear cc1 capture interrupt
   main_loop::timer::get()->sr.clearbit<1>(); //(CC1IF)

   // set capture parameters
   auto ccer_reg = main_loop::timer::get()->ccer.get();
   typedef decltype(ccer_reg) ccer_vt;
      // this bit must be kept cleared in output mode
      ccer_reg &= ~quan::bit<ccer_vt>(3); // (CC1NP)
      // output pin active high
      ccer_reg &= ~quan::bit<ccer_vt>(1); // (CC1P)
    
      ccer_reg &= ~quan::bit<ccer_vt>(0); // (CC1E)
   main_loop::timer::get()->ccer = ccer_reg;

   auto ccmr1_reg = main_loop::timer::get()->ccmr1.get();
   typedef decltype(ccmr1_reg) ccmr1_vt;
      // PWM mode 1
      ccmr1_reg |=  (static_cast<ccmr1_vt>(0b110) << 4); // OC1M
      ccmr1_reg &= ~(static_cast<ccmr1_vt>(0b001) << 4);  // OC1M
      //enable preload dat transferred on update event e.g. timer overflow
      ccmr1_reg |= quan::bit<ccmr1_vt>(3) ; // 
      // set CC1 as as output
      ccmr1_reg &= ~static_cast<ccmr1_vt>(0b011);
   // dont forget to assign the temporary back to the reg!
   main_loop::timer::get()->ccmr1 = ccmr1_reg;

   // initialise the compare reg to 0. to disable output till start?
   main_loop::timer::get()->ccr1 = 0;
   // enable compare output in ccer reg 
   main_loop::timer::get()->ccer.setbit<0>(); //(CC1E)
}

void azimuth::motor::setup_calc_compare()
{
   // enable the CC4IE interrupt
   main_loop::timer::get()->dier.setbit<4>(); // (CC4IE) interrupt
   // but disable DMA
   main_loop::timer::get()->dier.clearbit<12>(); // (CC4DE) dma
   // clear any interrupt
   main_loop::timer::get()->sr.clearbit<4>(); //(CC4IF)

    // set capture parameters
   auto ccer_reg = main_loop::timer::get()->ccer.get();
   typedef decltype(ccer_reg) ccer_vt;
      // this bit must be kept cleared in output mode
      ccer_reg &= ~quan::bit<ccer_vt>(15); // (CC4NP)
      // ########### disable Compare. This bit  must be cleared to write ccmr2 below##########
      ccer_reg &= ~quan::bit<ccer_vt>(12); // (CC4E)
      //###########################################################
   main_loop::timer::get()->ccer = ccer_reg;

   auto ccmr2_reg = main_loop::timer::get()->ccmr2.get();
   typedef decltype(ccmr2_reg) ccmr2_vt;
      // set CC4 as as Capture  mode
      ccmr2_reg &= ~(static_cast<ccmr2_vt>(0b011) << 8); // (CC4S)
      // frozen capture mode no output pins
      ccmr2_reg &= ~(static_cast<ccmr2_vt>(0b111) << 12); // (OC4M)
      // refman 14.4.7 states that 
      // may need to mod or write the ccr reg every cycle  to "validate"?
       // not sure quite what that means though!
      ccmr2_reg &= ~quan::bit<ccmr2_vt>(11) ; // (OC4PE)
     
   main_loop::timer::get()->ccmr2 = ccmr2_reg ;

   //load the compare register
   main_loop::timer::get()->ccr4 = period_before_calc;
   // re-enable compare
   main_loop::timer::get()->ccer.setbit<12>(); // (CC4E)
}

void azimuth::motor::set_kP(float val)
{
   m_kP = val;
}

void azimuth::motor::set_kD(float val)
{
    m_kD = val;
}

float azimuth::motor::get_kP()
{
    return m_kP;
}

float  azimuth::motor::get_kD()
{
    return m_kD;
}
