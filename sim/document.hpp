#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/uav/position.hpp>

struct tracker_params{
   tracker_params(   quan::length::m const & distance_in,
   quan::angle::deg const &  bearing_in,
   quan::angle::deg const & elevation_in
   )
  : distance{distance_in}, bearing{bearing_in},elevation{elevation_in}{}

   quan::length::m distance; //aircraft horizontal distance from home
   quan::angle::deg bearing; // bearing from home 
   quan::angle::deg elevation; // elevation of aircraft
};

struct document{
   document();

   static constexpr quan::length::m  			  radius_of_world{6371000};
   quan::uav::position<quan::angle::deg,quan::length::mm> const & 
   get_home_position()const{ return m_home_position;}
  // quan::uav::position const & get_aircraft_position()const { return m_aircraft_position;}
   quan::two_d::vect<quan::length::mm> const & 
   get_map_size() const {return m_map_size;}

   template <typename AngleType,typename LengthType>
   void  set_home_position(quan::uav::position<AngleType, LengthType> const & pos)
   {  m_home_position = pos;}
  // void  set_aircraft_position(quan::uav::position const & pos){ m_aircraft_position = pos;}
   void set_map_size( quan::two_d::vect<quan::length::mm> const & size)
   { m_map_size = size;}

   tracker_params get_tracker_params()const;

    // x,y,z position relative to home
   quan::three_d::vect<quan::length::mm> 
   get_aircraft_xyz_position()const { return m_aircraft_xyz_position;}

   // set alt relative to home
   void set_aircraft_alt(quan::length::m const & alt) { m_aircraft_xyz_position.z = alt;}
   // x,y,z all relative to home
   void set_aircraft_xyz_position(quan::three_d::vect<quan::length::mm> const & pos)
   {m_aircraft_xyz_position = pos;}

   template <typename AngleType, typename LengthType>
   quan::uav::position<AngleType,LengthType>
   get_aircraft_gps_position()const 
   { 
      return  xyz_to_gps_position<AngleType,LengthType>(m_aircraft_xyz_position);
   }

   template <typename AngleType, typename LengthType>
   quan::uav::position<AngleType,LengthType> xyz_to_gps_position(quan::three_d::vect<LengthType> const & in)const
   {
      LengthType rlat = cos(abs(m_home_position.lat)) * radius_of_world;
      AngleType lon = quan::angle::rad{in.x / rlat} + m_home_position.lon;
      AngleType lat =  quan::angle::rad{in.y / radius_of_world} + m_home_position.lat;
      return {lat,lon,in.z + m_home_position.alt};
   }
   quan::length::mm get_click_bucket()const { return m_click_bucket;}
   quan::angle::deg get_aircraft_heading()const { return m_aircraft_heading;}
   void set_aircraft_heading(quan::angle::deg const & heading){m_aircraft_heading = heading;}

   tracker_params m_remote_tracker_params;

   private:
   quan::uav::position<quan::angle::deg,quan::length::mm> m_home_position;
  // quan::uav::position m_aircraft_position;
   quan::three_d::vect<quan::length::mm> m_aircraft_xyz_position;
   quan::angle::deg m_aircraft_heading;
   quan::two_d::vect<quan::length::mm> m_map_size;
   quan::length::mm  m_click_bucket;
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
