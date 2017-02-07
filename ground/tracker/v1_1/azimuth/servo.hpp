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


#include <quan/angle.hpp>
#include <quan/reciprocal_time.hpp>
#include <quan/irq_atomic_buffer.hpp>

#include "system/tracker_resources.hpp"
#include "../azimuth/encoder.hpp"

/*
   bearings are in radians using unsinged modulus
   angular_velocity in radians per sec
*/

extern "C"  void TIM1_CC_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void setup();

struct azimuth_servo {

   enum class mode_t : uint8_t {
      pwm
      ,position
      ,velocity
      ,position_and_velocity
   };
   // stm32 timer. In practise thses arent directly interchangeable
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
   static void set_target(quan::angle::rad const &angle, rad_per_s const & velocity )
   {
      m_target_buffer.ex_irq_set({angle,velocity});
   }

   static quan::angle::rad    get_current_bearing();
   static rad_per_s           get_current_angular_velocity();
   static rad_per_s           get_update_angular_velocity_from_irq();
   static quan::angle::rad    get_target_bearing()
   { 
      return m_target_buffer.in_irq_get().bearing;
   }

   static rad_per_s get_target_angular_velocity()
   { 
      return m_target_buffer.in_irq_get().angular_velocity;
   }

   static bool is_enabled() { return m_enabled;}

   static bool set_pwm(float value);

   static void set_mode(mode_t mode) { m_mode = mode;}
   static mode_t get_mode() { return m_mode;}


/*
   get current position and speed and recalc pwm to get to new position and speed
*/
   static void set_kP(float val) { m_kP = val;}

   static void set_kD(float val) { m_kD = val;}

   static float get_kP() { return m_kP;}

   static float get_kD() { return m_kD;}

   private:

   /*
      the number of timer steps from update to start of calc interrupt
   */
   static constexpr uint32_t get_calc_compare_irq_value()
   {
      return static_cast<uint32_t>( ((m_pwm_period - m_max_calculation_time) / m_timer_resolution) - 0.9f );
   }
  
   static mode_t m_mode;

   static void setup();
   static void setup_timer();
   static void setup_update_interrupt();

   static void update_irq();
   static void position_irq();

   static void ll_update_irq();

   friend  void ::TIM1_CC_IRQHandler() ;
   friend  void ::setup();

   // double buffer target so can update atomically outsdie irq context
   struct target_t{

      target_t(quan::angle::rad const &bearing_in, rad_per_s const & angular_velocity_in)
      : bearing{bearing_in},angular_velocity{angular_velocity_in}{}

      quan::angle::rad     bearing;
      rad_per_s            angular_velocity; 
   };

   struct last_t{

      last_t(quan::angle::rad const & bearing_in, rad_per_s const & angular_velocity_in, quan::time::ms const & t_in)
      : bearing{bearing_in},angular_velocity{angular_velocity_in},at_time{t_in}{}

      quan::angle::rad     bearing;
      rad_per_s            angular_velocity;
      quan::time::ms       at_time;
   };

   static last_t  m_last;

   static quan::irq_atomic_buffer<target_t> m_target_buffer;

   static bool m_enabled;
   
   static bool m_is_reversed;

   static float  m_kP;
   static float  m_kD;
   
};


#endif // QUANTRACKER_GROUND_TRACKER_V1_1_AZIMUTH_SERVO_HPP_INCLUDED
