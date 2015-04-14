
#include <cstdio>
#include <quan/constrain.hpp>
#include <quan/uav/osd/features_api.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include "symbology.hpp"
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

void draw_lat_lon()
{
	char buf [30];
	pxp_type pos;
	font_ptr font = get_font(FontID::OSD_Charset);

	position_type const aircraft_position = get_aircraft_position();

	if ( osd_show_latitude() == true)
	{
	   pos = get_osd_latitude_position();

	   if ( font)
	   {
         quan::angle_<int32_t>::deg10e7 cur_latitude = aircraft_position.lat;
         double cur_latitude_double = cur_latitude.numeric_value()/1e7;
		 sprintf(buf,"%c%9.5f%c",131, cur_latitude_double, 176);
		 draw_text(buf,pos,font);
	   }
	}


	if ( osd_show_longitude() == true)
	{
	   pos = get_osd_longitude_position();

	   if ( font)
	   {
         quan::angle_<int32_t>::deg10e7 cur_longitude = aircraft_position.lon;
         double cur_longitude_double = cur_longitude.numeric_value()/1e7;
		 sprintf(buf,"%c%10.5f%c",132, cur_longitude_double, 176);
		 draw_text(buf,pos,font);
	   }
	}
}
