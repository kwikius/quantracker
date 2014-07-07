#include "fsk.hpp"

#include "events.hpp"
#include "aircraft.hpp"
#include <quan/tracker/zapp3_encode.hpp>

namespace {

   typedef quan::uav::position<
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::mm  // N.B changed to mm from old protocol
   > pos_type;

} //namespace

namespace fsk {

   void send_message()
   {
      pos_type pos {
         the_aircraft.location.gps_lat,
         the_aircraft.location.gps_lon,
         the_aircraft.location.gps_alt
      };
      
      uint8_t encoded[16];
      quan::tracker::zapp3_encode (pos,encoded);
      fsk::write((const char*)encoded, 16);
   }
}

namespace {

   periodic_event fsk_event {quan::time_<uint32_t>::ms{200U},fsk::send_message,true};

} //namespace

namespace fsk {

   void setup_event()
   {
      set_event (event_index::fsk,&fsk_event);
   }
 
}//fsk
 