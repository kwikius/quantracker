/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */


#include "aircraft.hpp"

namespace {

   quan::uav::position<  
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::m 
   >  aircraft_gps_position;

}

void update_aircraft_gps_position(     quan::uav::position<  
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::m 
   > const & p)
{
   aircraft_gps_position = p;
}  

 quan::uav::position<  
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::m 
   > const & get_aircraft_gps_position()
{
   return  aircraft_gps_position;
}
