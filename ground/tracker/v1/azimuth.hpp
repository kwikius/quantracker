#ifndef QUANTRACKER_AZIMUTH_HPP_INCLUDED
#define QUANTRACKER_AZIMUTH_HPP_INCLUDED

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

#include "resources.hpp"
#include "main_loop.hpp"
#include <quan/angle.hpp>

 extern "C" void TIM2_IRQHandler();
 extern "C" void setup();

struct azimuth {

   struct encoder{
           
      typedef azimuth_encoder_a_pin          a_pin;
      typedef azimuth_encoder_b_pin          b_pin;
      typedef azimuth_qdrt_counter           counter;
     
      static constexpr uint32_t counts_rev(){ return 9600;}
      static void setup();
      static void zero();
      static bool align_with_compass();
   };

   struct motor{
      static void enable();
      static void disable();
      static bool enabled();              
      static int32_t get_actual_position();
      static int32_t get_target_position();
      static void   set_target_position(uint32_t val); // should be atomic
      static void   set_azimuth(quan::angle::deg angle);
      static quan::angle::deg get_target_azimuth();
      static int32_t bearing_to_encoder( quan::angle::deg bearing);
      static quan::angle::deg encoder_to_bearing(uint32_t encoder_val);
      static void   set_kP(float val);
      static void   set_kD(float val);
      static float  get_kP();
      static float  get_kD();
      static float  get_min_duty_cycle() { return m_min_duty_cycle_percent;}
      static constexpr float  m_max_duty_cycle_percent = .9f;

      typedef azimuth_motor_direction_out_pin      direction_pin;
      typedef not_azimuth_motor_direction_out_pin  not_direction_pin;
      typedef azimuth_motor_pwm_out_pin            pwm_pin;

      static void do_calc();
   private:

      static constexpr quan::time::ms calc_time{3};
      static constexpr uint32_t calc_count = static_cast<uint32_t>((calc_time * main_loop::max_count) / main_loop::period);
      static constexpr uint32_t period_before_calc = main_loop::max_count - calc_count;

      static void setup();
      static void setup_ports();
      static void setup_pwm();
      static void setup_calc_compare();

      static bool             m_enabled;
      static int32_t          m_target_position;
      static float            m_min_duty_cycle_percent;

      static float            m_kP;
      static float            m_kD;
      static int32_t          m_last_recorded_position;

      friend void             ::TIM2_IRQHandler();
      friend struct           main_loop;
      friend void             ::setup();
   };
};

#endif // QUANTRACKER_AZIMUTH_HPP_INCLUDED
