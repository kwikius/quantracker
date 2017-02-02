
/*
 Copyright (c) 2013 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See theag137

 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include <quan/stm32/tim/temp_reg.hpp>

#include "../system/common_resources.hpp"
#include "../system/osd_resources.hpp"
#include "../azimuth/servo.hpp"
#include "../azimuth/motor.hpp"

/*
     using L298
     using TIM1_CH3N as H bridge enable pin
     use  also a direction and not direction pin to set the direction
      where not_direction is alway == ! direction whe the PWM is on
*/

float azimuth_motor::m_kP = 0.0045;
float azimuth_motor::m_kD = 0.01; // 0.001

uint8_t azimuth_motor::m_next_pwm_idx = 0U;
azimuth_motor::next_pwm_t azimuth_motor::next_pwm[] = {
   {0U,false},{0U,false}
};

using namespace quan::stm32;

void azimuth_motor::setup_ports()
{
   module_enable<pwm_pin::port_type>();
   module_enable<direction_pin::port_type>();
   module_enable<not_direction_pin::port_type>();

   apply<
      pwm_pin
      ,gpio::mode::af1     
      ,gpio::pupd::none  
      ,gpio::ospeed::slow 
   >();

   apply<
      direction_pin
      ,gpio::mode::output
      ,gpio::otype::push_pull
      ,gpio::ospeed::slow
      ,gpio::ostate::high
   >();

   apply<
      not_direction_pin
      ,gpio::mode::output
      ,gpio::otype::push_pull
      ,gpio::ospeed::slow
      ,gpio::ostate::low
   >();
}

void azimuth_motor::setup()
{
   setup_ports();
   setup_pwm();
}

void azimuth_motor::setup_pwm()
{
   typedef azimuth_servo::timer timer;

   timer::get()->ccr3 = 0;
   {
      quan::stm32::tim::ccmr2_t ccmr2 = timer::get()->ccmr2.get();
      ccmr2.cc3s = 0b00; // channel 3 output
      ccmr2.oc3m = 0b110;  // PWM mode 1
      ccmr2.oc3pe = true;  // if true, the new value is actually loaded to the live reg on next update
      timer::get()->ccmr2.set(ccmr2.value);
   }

   {
     // set up TIM1_CH3N polarity high pulse
      quan::stm32::tim::ccer_t ccer = timer::get()->ccer.get();
      ccer.cc3np = false; // TIM1_CH3N is positive pulse
      ccer.cc3e = false; // dont enable TIM1_CH3
      //#####################################################
      ccer.cc3ne = true; // dont enable TIM1_CH3N output yet
      //#####################################################
      timer::get()->ccer.set(ccer.value);
   }

   {
      quan::stm32::tim::bdtr_t bdtr = 0;
      bdtr.moe = true;  // main output enable
      bdtr.aoe = true;  // automatic output enable
      bdtr.ossi = true; // offs stet select, enable the outputs in idle
      bdtr.ossr = true; // enable the outputs in run mode
      bdtr.bke = false;   // disable break inputs
      bdtr.bkp = false;   // break polarity is dont care
      bdtr.dtg = 0;
      timer::get()->bdtr.set(bdtr.value);
   }

}

namespace {

  int irq_count = 0;
}

void azimuth_motor::set_pwm_irq()
{
     if (++irq_count == 50){
         irq_count =0;
         quan::stm32::complement<heartbeat_led_pin>();
     }
     bool const sign = next_pwm[m_next_pwm_idx].sign;
     quan::stm32::put<direction_pin>(sign);
     quan::stm32::put<not_direction_pin>(!sign);
     typedef azimuth_servo::timer timer;
     timer::get()->ccr3 = next_pwm[m_next_pwm_idx].value;
}

void azimuth_motor::set_pwm(uint32_t value, bool sign)
{
   next_pwm[m_next_pwm_idx ^ 1U] = {value,sign};
   m_next_pwm_idx ^= 1U;
}

void azimuth_motor::enable()
{
   static constexpr uint8_t cc3ne = 10U;
   typedef azimuth_servo::timer timer;

   timer::get()->ccer.bb_setbit<cc3ne>(); // set cc3NE to enable PWM out
}

void azimuth_motor::disable()
{
   static constexpr uint8_t cc3ne = 10U;
   typedef azimuth_servo::timer timer;

   timer::get()->ccer.bb_clearbit<cc3ne>(); // cleasr cc3NE to disable PWM out
}

/*
   get current position and speed and recalc pwm to get to new position and speed
*/
void azimuth_motor::set_kP(float val)
{
    m_kP = val;
}

void azimuth_motor::set_kD(float val)
{
    m_kD = val;
}

float azimuth_motor::get_kP()
{
    return m_kP;
}

float  azimuth_motor::get_kD()
{
    return m_kD;
}
