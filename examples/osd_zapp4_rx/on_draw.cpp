
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include "rx_telemetry.hpp"
//#include "resources.hpp"

uint32_t get_good_packets_received();

uint32_t get_packets_received();

namespace quan{ namespace uav { namespace osd{

   void on_draw()
   {
      pxp_type pos{-150,40};

      norm_position_type* gps_position_in = mutex_acquire_position(5);
      if ( gps_position_in != nullptr){
         norm_position_type gps_position = *gps_position_in;
         mutex_release_position();
         char buf[ 100];
         sprintf(buf,"lat = %8.3f deg",static_cast<double>(gps_position.lat.numeric_value())* 1e-7);
         draw_text(buf,pos);
         pos.y -= 20;

         sprintf(buf,"lon = %8.3f deg",static_cast<double>(gps_position.lon.numeric_value())*1e-7);
         draw_text(buf,pos);
         pos.y -= 20;

         sprintf(buf,"alt = %8.3f m",static_cast<double>(gps_position.alt.numeric_value() *1e-3) );
         draw_text(buf,pos);
//         pos.y -= 20;
//         unsigned const packets = get_packets_received();
//         unsigned const good_packets = get_good_packets_received();
//         sprintf(buf,"pkts = %u,gpkts = %u",packets, good_packets);
//         draw_text(buf,pos);
      }
   }

}}} // quan::uav::osd