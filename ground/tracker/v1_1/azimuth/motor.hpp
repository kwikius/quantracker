#ifndef QUANTRACKER_GROUND_V1_1_AZIMUTH_MOTOR_CPP_INCLUDED
#define QUANTRACKER_GROUND_V1_1_AZIMUTH_MOTOR_CPP_INCLUDED

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

#include "../system/tracker_resources.hpp"
#include <quan/irq_atomic_buffer.hpp>

struct azimuth_motor{

   static void enable();
   static void disable();
   typedef pan_motor_direction_out_pin      direction_pin;
   typedef pan_motor_not_direction_out_pin  not_direction_pin;
   typedef pan_motor_pwm_out_pin            pwm_pin;
private:
   friend struct azimuth_servo;
   static void set_pwm(uint32_t pwm_in, bool sign); 
   static void set_pwm_irq();
   static void setup();
   static void setup_ports();
   static void setup_pwm();
   struct next_pwm_t{
      uint32_t value;
      bool sign;
   };
   static quan::irq_atomic_buffer<next_pwm_t> next_pwm;
};

#endif // QUANTRACKER_GROUND_V1_1_AZIMUTH_MOTOR_CPP_INCLUDED
