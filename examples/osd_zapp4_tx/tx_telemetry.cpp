
/*
   see quan_matters/test/zapp4/main.cpp for test
*/
#include <quantracker/osd/telemetry_transmitter.hpp>
#include <quan/tracker/zapp4/position.hpp>

namespace {

   quan::uav::position<
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::mm  // 
   > position{quan::angle::deg{100.f},quan::angle::deg{150.f},quan::length::mm{ 300.f}};

}

void on_telemetry_transmitted()
{
   // N.B 117 is max number of bytes that can be transmitted in one half frame (todo check for NTSC)
   // this could be sent up to 5 x in a frame here
   uint8_t encoded [19];
   quan::tracker::zapp4::encode_position(position,encoded);
   write_telemetry_data((const char*)encoded,19);
}
