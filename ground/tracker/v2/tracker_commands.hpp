#ifndef QUANTRACKER_GROUND_V2_TRACKER_COMMANDS_HPP_INCLUDED
#define QUANTRACKER_GROUND_V2_TRACKER_COMMANDS_HPP_INCLUDED

#include <quan/angle.hpp>
#include <quan/voltage.hpp>
#include <quan/reciprocal_time.hpp>
#include <quan/frequency.hpp>
#include <quan/voltage.hpp>
#include <quan/current.hpp>
#include <quan/time.hpp>
  
/*
   move pan to  bearing b deg and angular velocity of alpha rad/s
   move tilt to angle b( rel horizontal) and angular velocity of beta rad/s
   tracker{
      enable_pan
      disable_pan
      enable_tilt
      disable_tilt
      get_pan_bearing
      get_pan_angular_velocity 
      get_pan_motor_current 
      get_tilt_angle
      get_tilt_angular_velocity 
     
   }
   
other
      get_battery_voltage
      read flas_variable
      write flash_variable
   
*/
   struct tracker{
      typedef quan::angle::deg deg;
      typedef quan::angle::rad rad;
      typedef quan::reciprocal_time_<rad>::per_s rad_per_s;
      struct pan{
         static void enable(bool b);
         static bool enabled();
         static deg get_angle(); // from mag or qdrt
         static rad_per_s get_angular_velocity();
         static void set_angular_velocity(rad_per_s const & v);
         static rad_per_s get_target_angular_velocity();
         static void move(deg const & target_angle, rad_per_s const & dtheta_at_target_bearing);
         static quan::voltage::mV get_back_emf_0v_rail();
         static quan::voltage::mV get_current_0v_rail();
         static void set_kP(float val);
         static void set_kD(float val);
         static void set_kC(float val);
      };
      struct tilt{
         static void enable(bool b);
         static bool enabled();
         static deg get_angle();
         static rad_per_s get_angular_velocity();
         static void move(deg const & target_angle, rad_per_s const & dtheta_at_target_bearing);
      };

      quan::voltage::V get_battery_voltage();
      
   };


#endif // QUANTRACKER_GROUND_V2_TRACKER_COMMANDS_HPP_INCLUDED
