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

#include "document.hpp"
#include <quan/atan2.hpp>

constexpr quan::length::m  			  document::radius_of_world;
document::document()
: m_remote_tracker_params{quan::length::m{0}, quan::angle::deg{0}, quan::angle::deg{0}}
,m_home_position{quan::angle::deg{51.86260},quan::angle::deg{-0.18625}, quan::length::mm{100000}}
,m_aircraft_xyz_position{quan::length::mm{0},quan::length::mm{0},quan::length::mm{0}}
,m_map_size{quan::length::mm{100000},quan::length::mm{100000}}
,m_click_bucket{quan::length::mm{3}}
{

}
// convert a gps coord to a vector position
//quan::three_d::vect<quan::length::mm> document::get_rel_aircraft_position()const
//{
//   quan::angle_<int32_t>::deg10e7  dlat = m_aircraft_position.lat - m_home_position.lat; // sort diffence for angle
//   quan::angle_<int32_t>::deg10e7  dlon = m_aircraft_position.lon - m_home_position.lon;
//
//   quan::length::mm  rlat = cos(abs(m_home_position.lat + m_aircraft_position.lat)/2) * radius_of_world;
//   
//   quan::length::mm  dx = quan::angle::rad{dlon} * rlat;
//   quan::length::mm  dy = quan::angle::rad{dlat} * radius_of_world;
//   quan::length::mm  dz = m_aircraft_position.alt - m_home_position.alt;
//
//   return {dx,dy,dz}; 
//}

tracker_params document::get_tracker_params()const
{

   auto pos = get_aircraft_xyz_position();
   quan::length::m  h_distance = quan::sqrt(pos.x * pos.x + pos.y * pos.y);
   if ( abs(h_distance) > quan::length::mm{100}){
       
      quan::angle::deg bearing = quan::atan2(-pos.y, pos.x) + quan::angle::pi/2;
      if ( bearing < quan::angle::deg{0}){
         bearing += quan::angle::deg{360};
      }
      quan::angle::deg elevation = quan::atan2(pos.z, h_distance);
      return { h_distance,bearing,elevation};
     
   }else{
      return {h_distance, quan::angle::deg{0}, quan::angle::deg{90}};
   }
}
/*
template <typename AngleType, typename LengthType>
quan::uav::position<AngleType,LengthType>
 document::xyz_to_gps_position(quan::three_d::vect<LengthType> const & in)const
{
   LengthType rlat = cos(abs(m_home_position.lat)) * radius_of_world;
   AngleType lon = quan::angle::rad{in.x / rlat} + m_home_position.lon;
   AngleType lat =  quan::angle::rad{in.y / radius_of_world} + m_home_position.lat;
  // quan::length::mm alt = in.z + 
   return {lat,lon,in.z + m_home_position.alt};
}
*/

