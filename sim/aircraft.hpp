#ifndef QUANTRACKER_SIM_AIRCRAFT_HPP_INCLUDED
#define QUANTRACKER_SIM_AIRCRAFT_HPP_INCLUDED

#include <quan/uav/position.hpp>

void update_aircraft_gps_position(     quan::uav::position<  
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::m 
   > const & p);

 quan::uav::position<  
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::m 
   > const & get_aircraft_gps_position();

#endif // QUANTRACKER_SIM_AIRCRAFT_HPP_INCLUDED
