
#include "gps.hpp"
#include "aircraft.hpp"
#include "flag.hpp"

flag new_gps_data;

namespace {
   static bool new_data = false;
   void on_gps(gps& g)
   {
       if ( g.m_latitude_deg10e7.changed()) {
           the_aircraft.location.gps_lat = quan::angle_<int32_t>::deg10e7{ g.m_latitude_deg10e7.get_value() };
           g.m_latitude_deg10e7.clear_change();
           new_gps_data.set();
       }

       if ( g.m_longtitude_deg10e7.changed()) {
           the_aircraft.location.gps_lon = quan::angle_<int32_t>::deg10e7{ g.m_longtitude_deg10e7.get_value()};
           g.m_longtitude_deg10e7.clear_change();
           new_gps_data.set();
       }

       if(g.m_fix_type.changed()) {
           the_aircraft.gps.fix_type = g.m_fix_type.get_value();
           g.m_fix_type.clear_change();
           new_gps_data.set();
       }
       
       if(g.m_sats_used.changed()){
          the_aircraft.gps.num_sats = g.m_sats_used.get_value();
          g.m_sats_used.clear_change();
          new_gps_data.set();
       }

       if(g.m_altitude_mm.changed()){
         the_aircraft.location.gps_alt = quan::length_<int32_t>::mm{g.m_altitude_mm.get_value()};
         g.m_altitude_mm.clear_change();
         new_gps_data.set();
       }
   }
}
gps the_gps( on_gps);