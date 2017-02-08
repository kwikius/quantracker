#ifndef QUANTRACKER_GROUND_TRACKER_V1_1_SERVO_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V1_1_SERVO_HPP_INCLUDED

#include "system/tracker_resources.hpp"
#include <quan/angle.hpp>
/*

elevation servo uses same timer as azimuth servo
uses TIM1_CH4 which timing is already setup by azimuth_servo
*/

struct elevation_servo{

   static void setup();
   static bool enable();
   static void disable();
   static void set_elevation(quan::angle::rad const &  angle);

};

#endif // QUANTRACKER_GROUND_TRACKER_V1_1_SERVO_HPP_INCLUDED
