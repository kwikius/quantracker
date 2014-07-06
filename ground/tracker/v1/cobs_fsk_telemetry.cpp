
#include <quan/config.hpp>
#include <quan/uav/fletcher16.hpp>
#include <quan/tracker/zapp3_decode.hpp>
#include "serial_ports.hpp"
#include "telemetry.hpp"

namespace {

   typedef quan::uav::position<
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::mm
   > pos_type;

   bool do_frame( pos_type & new_pos)
   {
      if (! av_fsk::serial_port::in_avail()) {
         return false;
      }
      static uint8_t ar[16];
      static uint8_t idx = 0;
      
      uint8_t ch = av_fsk::serial_port::get();
      if (idx != 0) { // in frame
         if (ch != 0U) {
            ar[idx] = ch;
            if (++idx == 16U) {
               idx = 0U; // reset
               return quan::tracker::zapp3_decode (ar,new_pos);
            } else {
               return false;
            }
         } else { // A zero in the encoded data means corruption
            idx = 0U;
            return false;
         }
      } else { // looking for start of frame
         if (ch == 0) { // start of frame
            ar[0] = 0U;
            idx = 1U;
         }
         return false;
      }
   }

} // namespace

void telemetry::parse_av_cobs()
{
  // read av sp
   pos_type new_pos;
   if (do_frame(new_pos) ){
       telemetry::m_aircraft_position = new_pos;
       telemetry::state_changed = true;
   }
}
