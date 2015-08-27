
#include <cstdio>
#include <cstring>
#include <quantracker/osd/telemetry_transmitter.hpp>
#include <quan/stm32/millis.hpp>

/*
   custom callback from the video
   previous telemetry data transmitted so
   put new telemetry data in buffer
*/

void on_telemetry_transmitted()
{
#if 1
   quan::time_<int64_t>::ms time_now = quan::stm32::millis();
   quan::time_<int>::min min_now{static_cast<int>(time_now.numeric_value() / 60000)};
   quan::time_<int>::s s_now{
      static_cast<int>((time_now.numeric_value()/1000) - (min_now.numeric_value() * 60))};
#endif
   char buffer[117]; // 117 is max number of bytes that can be transmitted in one half frame
   // (13 rows at 9 bytes per row)
#if 1
   snprintf(buffer,100,"time = %03d min %02d s", 
         static_cast<int>(min_now.numeric_value()),
         static_cast<int>(s_now.numeric_value()));
#else
   //snprintf(buffer,117,"
   memset(buffer,117,0);
#endif

   write_telemetry_data(buffer, 117);
}
