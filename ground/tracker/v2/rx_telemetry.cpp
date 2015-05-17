#include "rx_telemetry.hpp"
#include "../../../air/osd/video/video_buffer.hpp"
#include <cstring>

rx_telemetry the_rx_telemetry;

rx_telemetry::rx_telemetry():m_buffer{nullptr},m_buffer_length{0}{}
// must be called after video_setup() 
void rx_telemetry::init()
{ 
   m_buffer_length = video_buffers::telem::rx::get_num_data_bytes();
   m_buffer = (char*) pvPortMalloc (m_buffer_length);
   memset(m_buffer,0,m_buffer_length);
   this->mutex_init();
}

void rx_telemetry::mutex_init()
{
   m_mutex = xSemaphoreCreateMutex();
}

void rx_telemetry::mutex_acquire()
{
   xSemaphoreTake(m_mutex,portMAX_DELAY);
}

void rx_telemetry::mutex_release()
{
   xSemaphoreGive(m_mutex);
}

// called by receive_telemetry_task 
// when new data has been acquired
// The data is copied in to the_telemetry buffer
bool rx_telemetry::refresh()
{
   this->mutex_acquire();
   bool result = (m_buffer_length == video_buffers::telem::rx::get_num_data_bytes());
   if (result){
      memcpy(m_buffer,&video_buffers::telem::rx::manager.m_read_buffer->front(),m_buffer_length);
   }
   this->mutex_release();
   return result;
}

bool rx_telemetry::read(char * buffer, size_t len)
{
   if (m_buffer == nullptr){
      return false;
   }
   if ( m_buffer_length == 0){
      return false;
   }
   this->mutex_acquire();
   bool const result = (len <= m_buffer_length);
   if (result){
      memcpy(buffer,this->m_buffer,len);
   }
   this->mutex_release();
   return result;
}
