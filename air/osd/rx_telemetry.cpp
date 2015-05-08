#include "rx_telemetry.hpp"
#include "video/video_buffer.hpp"
#include <cstring>

rx_telemetry the_rx_telemetry;

rx_telemetry::rx_telemetry():m_buffer{nullptr},m_buffer_length{0}{}
// must be called after video_setup() 
void rx_telemetry::init()
{ 
   m_buffer_length = video_buffers::telem::rx::get_num_data_bytes();
   m_buffer = (char*) pvPortMalloc (m_buffer_length);
   memset(m_buffer,0,m_buffer_length);
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

// if len ! same as buffer_length
// dont copy and return false
// would only be due to some drastic mods to video mode
// telem baud etc
bool rx_telemetry::read(char * buffer, size_t len)
{
   this->mutex_acquire();
   bool const result = (len == m_buffer_length);
   if (result){
      memcpy(buffer,this->m_buffer,m_buffer_length);
   }
   this->mutex_release();
   return result;
}
