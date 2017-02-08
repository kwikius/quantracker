#ifndef QUANTRACKER_GROUND_TRACKER_V1_1_SERVO_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V1_1_SERVO_HPP_INCLUDED

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
