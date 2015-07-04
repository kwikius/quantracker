
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
   quan::time_<int64_t>::ms time_now = quan::stm32::millis();
   quan::time_<int>::min min_now{static_cast<int>(time_now.numeric_value() / 60000)};
   quan::time_<int>::s s_now{
      static_cast<int>((time_now.numeric_value()/1000) - (min_now.numeric_value() * 60))};
   char buffer[100];

   snprintf(buffer,100,"time = %03d min %02d s", 
         static_cast<int>(min_now.numeric_value()),
         static_cast<int>(s_now.numeric_value()));

   write_telemetry_data(buffer, strlen(buffer)+1);
}
