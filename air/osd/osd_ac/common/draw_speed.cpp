
#include <cstdio>
#include <quan/constrain.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

void draw_speed()
{
   

char buf [30];
pxp_type pos;
font_ptr font = get_font(FontID::OSD_Charset);


#if ((AIRSPEED_ENABLE & ENABLE_ON_PAGE_ALL) != 0)
   pos =
        {AIRSPEED_X,
        (( get_video_mode() == video_mode::pal)
        ?AIRSPEED_Y_PAL:AIRSPEED_Y_NTSC)};

   if ( font)
   {
	 quan::velocity_<double>::m_per_s cur_airspeed = get_airspeed();
     sprintf(buf,"%4.1f%c",cur_airspeed.numeric_value(),136);
     draw_text(buf,pos,font);

   }
#endif


#if ((GROUNDSPEED_ENABLE & ENABLE_ON_PAGE_ALL) != 0)
   pos =
        {GROUNDSPEED_X,
        (( get_video_mode() == video_mode::pal)
        ?GROUNDSPEED_Y_PAL:GROUNDSPEED_Y_NTSC)};

   if ( font)
   {
	 quan::velocity_<double>::m_per_s cur_groundspeed = get_groundspeed();
     sprintf(buf,"%4.1f%c",cur_groundspeed.numeric_value(),136);
     draw_text(buf,pos,font);

   }
#endif

}
