
#include <cstring>
#include <quan/uav/osd/api.hpp>
#include "../../examples/osd_example1/board/font.hpp"

#include "rx_telemetry.hpp"

namespace{

   // buffer is bigger here than necessary think at 2 Mbaud the num bytes == 117
   // implement on_draw
   char telem_buffer [128] = "0123456789";
   quan::uav::osd::font_ptr def_font = nullptr;
}

namespace quan{ namespace uav { namespace osd{

   void on_draw(){

      if (def_font == nullptr){
         def_font = get_font(FontID::OSD_Charset);
      }

      pxp_type pos{-150,65};
      draw_text("Receiver",pos,def_font);

      const char* telemetry_text = mutex_acquire_telemetry_string();

      if ( telemetry_text != nullptr){
         strcpy(telem_buffer,telemetry_text);
         mutex_release_telemetry_string();
         pxp_type text_pos{-150,-50};
         draw_text(telem_buffer,pos,def_font);
      }
   }
}}}