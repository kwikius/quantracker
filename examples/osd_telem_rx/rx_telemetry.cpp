
#include "FreeRTOS.h"
#include <semphr.h>

#include <quan/stm32/millis.hpp>
#include <quan/stm32/gpio.hpp>

#include "resources.hpp"
#include "rx_telemetry.hpp"

namespace {
   SemaphoreHandle_t m_mutex = NULL;
   char m_telemetry_string[200] = "-------------------";
   quan::time_<int64_t>::ms telemetry_received_time{0ULL};
}

const char* mutex_acquire_telemetry_string()
{
   if ( m_mutex != NULL){
      if(xSemaphoreTake(m_mutex,(TickType_t)20) == pdTRUE){
         return m_telemetry_string;
      }
   }
   return nullptr;
}

void mutex_release_telemetry_string()
{
   if ( m_mutex != NULL){
      xSemaphoreGive(m_mutex);
   }
}

quan::time_<int32_t>::ms get_telemetry_received_time()
{
   return telemetry_received_time;
}
namespace {
    int count = 49;
}
void on_telemetry_received()
{
   if ( m_mutex == NULL){
      m_mutex = xSemaphoreCreateMutex();
   }

   if (mutex_acquire_telemetry_string() != nullptr){
      read_telemetry_data(m_telemetry_string,200);
      m_telemetry_string[199]= '\0';
      telemetry_received_time = quan::stm32::millis();
      mutex_release_telemetry_string();
     
      if (++count == 50){
         count = 0;
         quan::stm32::complement<heartbeat_led_pin>();
      }
   }

}
