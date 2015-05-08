#include <quan/uav/osd/api.hpp>


namespace quan{ namespace uav { namespace osd{

  void on_draw(){
 #if defined QUAN_OSD_TELEM_RECEIVER
  pxp_type pos  ={-50,50};
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
  }

}}}