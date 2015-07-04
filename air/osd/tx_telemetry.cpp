
#include <cstdio>
#include <cstring>
#include <quantracker/osd/telemetry_transmitter.hpp>
#include <quan/stm32/millis.hpp>

/*
   custom callback from the video
   previous telemetry data transmitted so
   put new telemetry data in buffer
*/
/*
   Just send data from a timer here, but can be used to show
   transmitter is working by 
*/
void on_telemetry_transmitted()
{
   int64_t time_now = xTaskGetTickCount() * (1000 / configTICK_RATE_HZ); //( ms)
   int min_now = static_cast<int>(time_now / 60000);
   int s_now   = static_cast<int>((time_now / 1000) - (min_now * 60));
   char buffer[100];
   snprintf(buffer,100,"time = %03d min %02d s", min_now,s_now);
   write_telemetry_data(buffer, strlen(buffer)+1);
}
