#include "aircraft.hpp"
#include <quan/uav/osd/api.hpp>
#include <quan/uav/osd/features_api.hpp>
#include <quan/uav/osd/basic_bitmap.hpp>
#include <quan/uav/osd/basic_font.hpp>

#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/osd/get_aircraft_attitude.hpp>
#include <quan/uav/osd/get_aircraft_heading.hpp>


    quan::velocity_<float>::m_per_s
     quan::uav::osd::get_airspeed()
     {
       // atomic
         return the_aircraft.airspeed;
     }

    quan::velocity_<float>::m_per_s
     quan::uav::osd::get_groundspeed()
     {
       // atomic
         return the_aircraft.groundspeed;
     }

   quan::current_<float>::A  quan::uav::osd::get_battery_current(uint8_t n)
    {
       return the_aircraft.battery_current;
    }

   float quan::uav::osd::get_battery_remaining(uint8_t n)
    {
       return the_aircraft.battery_remaining;
    }

   uint8_t quan::uav::osd::get_base_mode()
    {
       //atomic
       return the_aircraft.base_mode;

    }

   uint8_t quan::uav::osd::get_custom_mode()
    {
       //atomic
       return the_aircraft.custom_mode;

    }

   uint16_t quan::uav::osd::get_rssi()
   {
      return the_aircraft.rc_raw_rssi;
   }

   uint16_t quan::uav::osd::get_rc_raw_chan(uint8_t channel)
   {
      return the_aircraft.rc_raw_chan[channel];
   }

   quan::length_<float>::m
   quan::uav::osd::get_baro_alt()
        {
          // atomic
            return the_aircraft.baro_alt;
        }
