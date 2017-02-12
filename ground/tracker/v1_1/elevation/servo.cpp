
/*
 Copyright (c) 2017 Andy Little 

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

#include <quan/max.hpp>
#include <quan/min.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/stm32/gpio.hpp>
#include "../elevation/servo.hpp"
#include "../azimuth/servo.hpp"

namespace {
   QUAN_ANGLE_LITERAL(rad)
   QUAN_ANGLE_LITERAL(deg)
   QUAN_QUANTITY_LITERAL(time,us)
}

void elevation_servo::set_elevation(quan::angle::rad const & angle_in)
{
   auto const angle = quan::max(quan::min(signed_modulo(angle_in), 90_deg),-20_deg);
   auto constexpr pulse_0  = 1088_us;
   auto constexpr pulse_90 = 1911_us;
   auto const pulse_time = pulse_0 + (pulse_90 - pulse_0) * angle / 90_deg;
   
   typedef azimuth_servo::timer timer;
   timer::get()->ccr4 = static_cast<uint32_t>(pulse_time / azimuth_servo::m_timer_resolution) ;
}

using namespace quan::stm32;

/*
  // TIM1_CH4 PA11  assume basic timer is already set
*/
void elevation_servo::setup()
{
   module_enable<elevation_servo_pwm_out_pin::port_type>();

   apply<
      elevation_servo_pwm_out_pin
      ,gpio::mode::af1     
      ,gpio::pupd::none  
      ,gpio::ospeed::slow 
   >();
   
   typedef azimuth_servo::timer timer;

   timer::get()->ccr4 = 0;
   {
      tim::ccmr2_t ccmr2 = timer::get()->ccmr2.get();
         ccmr2.cc4s = 0b00;   // channel 4 output
         ccmr2.oc4m = 0b110;  // PWM mode 1
         ccmr2.oc4pe = true;  // if true, the new value is actually loaded to the live reg on next update
      timer::get()->ccmr2.set(ccmr2.value);
   }
   {  // set up TIM1_CH4 polarity high pulse
      tim::ccer_t ccer = timer::get()->ccer.get();
         ccer.cc4p = false; // TIM1_CH4 is positive pulse ( active high)
         ccer.cc4e = false; // dont enable TIM1_CH4 yet
      timer::get()->ccer.set(ccer.value);
   }
}

bool elevation_servo::enable()
{
   typedef azimuth_servo::timer timer;
   constexpr uint8_t cc4e = 12U;  // ch 4 en
   timer::get()->ccer.bb_setbit<cc4e>();
   return true;
}

void elevation_servo::disable()
{
   typedef azimuth_servo::timer timer;
   constexpr uint8_t cc4e = 12U;  // ch 4 en
   timer::get()->ccer.bb_clearbit<cc4e>();
}
