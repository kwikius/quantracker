
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include "rx_telemetry.hpp"

bool packet_good();

namespace quan{ namespace uav { namespace osd{

   void on_draw()
   {
      pxp_type pos{-150,10};

      if ( get_num_telem_errors() != 0){
         clear_telem_errors();
         draw_box(pos, pos + pxp_type{15,15}, colour_type::black, true);

      }
   }

}}} // quan::uav::osd