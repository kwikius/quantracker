#include <cstdint>
#include <cassert>
#include <cstring>

#include <quan/tracker/zapp3_encode.hpp>
#include "aircraft.hpp"
#include "app.h"
 
// for 1/5 sec refresh rate
void COBS_send_message()
{
   uint8_t encoded[16];
   quan::tracker::zapp3_encode (get_aircraft_gps_position(),encoded);
   
   auto & app =wxGetApp();
   assert (app.have_sp());
   auto sp = app.get_sp();
   sp->write (encoded,16);
}
 
 
 
 
 