#ifndef QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED

#include "FreeRTOS.h"
#include <semphr.h>

void on_telem_receive();
void create_telem_rx_task();
// stores the latest telemetry data
// automatically refreshed by the osd telemetry susbsystem
// 
struct rx_telemetry{
   rx_telemetry();
   // TODO add a reinit if buffer size changed
   size_t get_buffer_length()const {return m_buffer_length;}
   bool read(char * buffer, size_t len);
private:
   char* m_buffer;
   void mutex_acquire();
   void mutex_release();
   bool refresh();
   // call once after video_setup() since only then is buffer size known
   void init();
   void mutex_init();
   SemaphoreHandle_t m_mutex;
   size_t m_buffer_length;
   friend void on_telem_receive();
   friend void create_telem_rx_task();
   // do a reinit if data speed changes
   
};

extern rx_telemetry the_rx_telemetry;

#endif // QUANTRACKER_AIR_OSD_TELEMETRY_HPP_INCLUDED
