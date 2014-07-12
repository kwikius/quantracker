

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