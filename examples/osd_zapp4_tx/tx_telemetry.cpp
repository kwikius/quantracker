
#include "tx_telemetry.hpp"

void on_telemetry_transmitted()
{
   // N.B 117 is max number of bytes that can be transmitted in one half frame (todo check for NTSC)
   // this could be sent up to 5 x in a frame here
   quan::uav::osd::norm_position_type * pos_ptr = mutex_acquire_position(5);
   if(pos_ptr){
      quan::uav::osd::norm_position_type pos_out = *pos_ptr;
      mutex_release_position();
      uint8_t encoded [19];
      quan::tracker::zapp4::encode_position(pos_out,encoded);
      write_telemetry_data((const char*)encoded,19);
   }
}

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
