#include "rx_telemetry.hpp"
#include "FreeRTOS.h"
#include <semphr.h>
#include <quan/stm32/millis.hpp>
// implement the receive telemetry callback

namespace {
   SemaphoreHandle_t m_mutex = NULL;
   const char m_telemetry_string[200] = "no data received";
   quan::time_<uint64_t>::ms telemetry_received_time{0ULL};
}

const char* mutex_acquire_telemetry_string()
{
   if(xSemaphoreTake(m_mutex,(TickType_t)10) == pdTrue){
      return m_telemetry_string;
   }else{
      return nullptr;
   }
}

bool mutex_release_telemetry_string()
{
   xSemaphoreGive(m_mutex);
}

quan::time_<uint32_t> get_telemetry_received_time()
{
   return time_diff;
}

void on_telemetry_received()
{
   if ( m_mutex == NULL){
      m_mutex = xSemaphoreCreateMutex();
   }
   if (mutex_lock_telemetry_string() ){
      read_telemetry_data(m_telemetry_string,200);
      m_telemetry_string[199]= '\0';
      telemetry_received_time = quan::stm32::millis();
      mutex_release_telemetry_string();
   }
}
