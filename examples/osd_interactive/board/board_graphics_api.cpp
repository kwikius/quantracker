
#include <quan/uav/osd/api.hpp>
#include <quan/uav/osd/features_api.hpp>
#include <quan/uav/osd/basic_bitmap.hpp>
#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/osd/get_aircraft_attitude.hpp>
#include <quan/uav/osd/get_aircraft_heading.hpp>
#include "../common/aircraft.hpp"

   quan::uav::osd::position_type
   quan::uav::osd::get_aircraft_position()
    {
       the_aircraft.mutex_acquire(); 
         quan::uav::osd::position_type const p = the_aircraft.location;
       the_aircraft.mutex_release();
       return p;
    }

    quan::uav::osd::position_type 
    quan::uav::osd::get_home_position()
    {
      the_aircraft.mutex_acquire();
         quan::uav::osd::position_type const p = the_aircraft.home_location;
      the_aircraft.mutex_release();
      return p;
    }

    quan::uav::osd::attitude_type
    quan::uav::osd::get_aircraft_attitude()
    {
       the_aircraft.mutex_acquire();
         quan::uav::osd::attitude_type const a= the_aircraft.attitude;
       the_aircraft.mutex_release();
       return a;
    }

    quan::uav::osd::angle_type 
    quan::uav::osd::get_aircraft_heading()
    {
      // atomic
        return the_aircraft.heading;
    }


   bool quan::uav::osd::position_is_good()
   {
      // atomic
      return (the_aircraft.gps.fix_type > 1);
   }

   namespace {
      bool home_position_set = false;
   }

   bool quan::uav::osd::home_position_is_set()
   {
      return home_position_set == true;
   }

   // even with force position must be good
   // use force to set home pos in flight
   bool quan::uav::osd::set_home_position(position_type const & p, bool force)
   {
       if ( ((home_position_set == false) || (force == true)) &&
           (quan::uav::osd::position_is_good()) ){
          the_aircraft.mutex_acquire();
            the_aircraft.home_location = p;
          the_aircraft.mutex_release();
          home_position_set = true;
          return true;
       }else{
          return false;
       }
   }

   uint8_t quan::uav::osd::read_gps_fix_type()
   {
      //atomic
      return the_aircraft.gps.fix_type;

   }
   uint8_t quan::uav::osd::read_gps_num_sats()
   {
       // atomic
       return the_aircraft.gps.num_sats;
   }

   // for now!
   uint8_t quan::uav::osd::get_num_batteries()
   {
      return 1;
   }
   const char* const quan::uav::osd::get_battery_name(uint8_t n)
   {
      return "Flt1";
   }

   quan::voltage_<float>::V  quan::uav::osd::get_battery_voltage(uint8_t n)
   {
      return the_aircraft.battery_voltage;
   }
