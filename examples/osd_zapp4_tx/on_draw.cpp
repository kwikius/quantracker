
#include <cstring>
#include <quan/uav/osd/api.hpp>
#include "../../examples/osd_example1/board/font.hpp"

#if defined QUAN_OSD_TELEM_RECEIVER
#include "rx_telemetry.hpp"
#endif

namespace{

   quan::uav::osd::font_ptr def_font = nullptr;
}

namespace quan{ namespace uav { namespace osd{

   void on_draw(){
        if (def_font == nullptr){
          def_font = quan::uav::osd::get_font(FontID::OSD_Charset);
        }
        pxp_type pos{-150,100};
        draw_text("Transmitter",pos,def_font);
   }
}}}