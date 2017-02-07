#ifndef QUANTRACKER_GROUND_V1_1_AZIMUTH_MOTOR_CPP_INCLUDED
#define QUANTRACKER_GROUND_V1_1_AZIMUTH_MOTOR_CPP_INCLUDED

#include "../system/tracker_resources.hpp"
#include <quan/irq_atomic_buffer.hpp>

struct azimuth_motor{

   static void enable();
   static void disable();
  // static bool is_enabled();             

   // kP is constant proportional bearing error
   

 
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
