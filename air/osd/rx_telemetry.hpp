#ifndef QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED

#include "FreeRTOS.h"
#include <semphr.h>

// keep public
 void create_vsync_telem_rx_task();

// on_telemetry_transmitted
//#############################
 // rename  to on_telemetry_received
 void on_telemetry_receive();
//############################
 size_t get_telemetry_num_bytes();
 size_t read_telemetry_data(char * buffer, size_t len);

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
   friend void on_telemetry_receive();
   friend void create_vsync_telem_rx_task();
   // do a reinit if data speed changes
};

extern rx_telemetry the_rx_telemetry;

#endif // QUANTRACKER_AIR_OSD_TELEMETRY_HPP_INCLUDED
