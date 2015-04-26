
#include <cstdio>
#include <quan/constrain.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "symbology.hpp"
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

void draw_speed()
{
   

char buf [30];
pxp_type pos;
font_ptr font = get_font(FontID::OSD_Charset);


	if ( osd_show_airspeed() == true)
	{
	   pos = get_osd_airspeed_position();

	   if ( font)
	   {
		 quan::velocity_<double>::m_per_s cur_airspeed = get_airspeed();
		 sprintf(buf,"%4.1f%c",cur_airspeed.numeric_value(),136);
		 draw_text(buf,pos,font);

	   }
	}


	if ( osd_show_groundspeed() == true)
	{
	   pos = get_osd_groundspeed_position();

	   if ( font)
	   {
		 quan::velocity_<double>::m_per_s cur_groundspeed = get_groundspeed();
		 sprintf(buf,"%4.1f%c",cur_groundspeed.numeric_value(),136);
		 draw_text(buf,pos,font);

	   }
	}

}
