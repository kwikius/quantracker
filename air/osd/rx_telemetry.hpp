#ifndef QUANTRACKER_AIR_OSD_TELEMETRY_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_TELEMETRY_HPP_INCLUDED

#include "FreeRTOS.h"
#include <semphr.h>

void on_telem_receive();
struct rx_telemetry{
   rx_telemetry();
   char* m_buffer;
   // call once after video_setup() since only then is buffer size known
   void init();
   // TODO add a reinit if buffer size changed
   size_t get_buffer_length()const {return m_buffer_length;}
   bool read(char * buffer, size_t len);
private:
   void mutex_acquire();
   void mutex_release();
   bool refresh();
   void mutex_init();
   SemaphoreHandle_t m_mutex;
   size_t m_buffer_length;
   friend void on_telem_receive();
   // do a reinit if data speed changes
   
};

extern rx_telemetry the_rx_telemetry;

#endif // QUANTRACKER_AIR_OSD_TELEMETRY_HPP_INCLUDED
