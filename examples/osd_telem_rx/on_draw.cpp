
#include <cstring>
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include "../../examples/osd_example1/board/font.hpp"
#include <quan/stm32/gpio.hpp>
#include "rx_telemetry.hpp"
#include "resources.hpp"

namespace{

   char telem_buffer [50] = "0123456789";

}

namespace quan{ namespace uav { namespace osd{

   void on_draw()
   {
      draw_text("Receiver",{-150,65});

      vTaskSuspendAll();
      const char* telemetry_text = mutex_acquire_telemetry_string();
      if ( telemetry_text != nullptr){
   
         strncpy(telem_buffer,telemetry_text,49);
         telem_buffer[49] = '\0';
         mutex_release_telemetry_string();
         xTaskResumeAll();
        
         draw_text(telem_buffer,{-170,-10});

         auto time = get_telemetry_received_time();
         char buf[30];
         snprintf(buf,29,"recvd at: %u",static_cast<unsigned int>(time.numeric_value()));
         buf[29] = '\0';
         draw_text(buf,{-170,-30});
      }else{
         xTaskResumeAll();
      }
      
   }

}}} // quan::uav::osd