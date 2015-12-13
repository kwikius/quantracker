#include "rx_telemetry.hpp"


namespace {
   quan::uav::osd::norm_position_type position;
   SemaphoreHandle_t m_position_mutex = NULL;
}

quan::uav::osd::norm_position_type * mutex_acquire_position(TickType_t ms)
{
   if( ( m_position_mutex != NULL ) && ( xSemaphoreTake(m_position_mutex,ms) == pdTRUE ) ){
     return & position;
   }else{
      return nullptr;
   }
}

void  mutex_release_position()
{
   if( m_position_mutex != NULL ){
      xSemaphoreGive(m_position_mutex);
   }
}

void setup_telemetry_values()
{
   if(m_position_mutex == NULL){
      m_position_mutex = xSemaphoreCreateMutex();
   }
}


