#ifndef QUANTRACKER_GROUND_TRACKER_V1_1_AZIMUTH_SERVO_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V1_1_AZIMUTH_SERVO_HPP_INCLUDED

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

#include "system/tracker_resources.hpp"

#include <quan/angle.hpp>
#include <quan/reciprocal_time.hpp>

extern "C"  void TIM1_CC_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void setup();

struct azimuth_servo {

   typedef pan_motor_timer                  timer;
   // motor pwm period
   static constexpr quan::time::us  m_pwm_period{20000U};
   // set the prescaler resolution
   static constexpr quan::time::ns  m_timer_resolution{1000U};
   // the time required to do the pid calculation and set the next PWM value
   //  before the start of the next PWM cycle
   // need to test that
   static constexpr quan::time::us m_max_calculation_time{100U};

   typedef quan::reciprocal_time_<quan::angle::rad>::per_s rad_per_s;
   static bool enable();
   static void disable();
   static void   set_target(quan::angle::rad const &angle, rad_per_s const & velocity )
   {
      target_arr[m_current_target_idx^1] = {angle,velocity};
      m_current_target_idx ^= 1;  // atomic update
   }
   static quan::angle::rad    get_current_angle();
   static rad_per_s           get_current_velocity();
   static quan::angle::rad   get_target_angle_from_irq()
   { 
      return target_arr[m_current_target_idx].angle;
   }
   static rad_per_s   get_target_velocity_from_irq()
   { 
      return target_arr[m_current_target_idx].velocity;
   }

   static bool is_enabled() { return m_enabled;}

   static void set_pwm(float value);

   private:

   static constexpr uint32_t get_calc_compare_irq_value()
   {
     return static_cast<uint32_t>( ((m_pwm_period - m_max_calculation_time) / m_timer_resolution) - 0.9f );
   }


   static void setup();
   static void setup_timer();
   static void setup_update_interrupt();

   static void   update_irq();

   static void ll_update_irq();

   friend  void ::TIM1_CC_IRQHandler() ;
   friend  void ::setup();

   // double buffer target so can update atomically outsdie irq context
   struct targets_t{
      targets_t(quan::angle::rad const &angle_in, rad_per_s const & velocity_in)
      : angle{angle_in},velocity{velocity_in}{}
      quan::angle::rad angle;
      rad_per_s    velocity; 
   };

   static         targets_t target_arr[2];
   static uint8_t m_current_target_idx;

   static bool m_enabled;
};


#endif // QUANTRACKER_GROUND_TRACKER_V1_1_AZIMUTH_SERVO_HPP_INCLUDED
