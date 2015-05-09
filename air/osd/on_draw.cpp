#include <quan/uav/osd/api.hpp>

#if defined QUAN_OSD_TELEM_RECEIVER
#include "rx_telemetry.hpp"
#endif

namespace quan{ namespace uav { namespace osd{

   char t_buffer [1000];
   
   void on_draw(){
      #if defined QUAN_OSD_TELEM_RECEIVER
      pxp_type pos  ={50,50};
      text_ptr text = "Receiver";
      #elif defined QUAN_OSD_TELEM_TRANSMITTER
      pxp_type pos  ={-50,50};
      text_ptr text = "Transmitter";
      #else
      pxp_type pos  ={0,0};
      text_ptr text = "No telem";
      #endif
      font_ptr font = get_font(0);
      if (font != nullptr){
         draw_text(text,pos,font);
      }
      #if defined QUAN_OSD_TELEM_RECEIVER
         pxp_type pos1{-150,-50};
         auto len = the_rx_telemetry.get_buffer_length();
        if ( the_rx_telemetry.read(t_buffer,len)){
            if ( t_buffer[0] != '\0'){
               t_buffer[len] = '\0';
               draw_text(t_buffer,pos1,font);
            }else{
               draw_text("no telem data received",pos1,font);
            }
        }
      #endif
      
   }

}}}