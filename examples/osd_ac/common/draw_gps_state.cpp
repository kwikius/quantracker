
#include <cstdio>
#include <cstring>
#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/features_api.hpp>
#include <quan/constrain.hpp>
#include "on_draw.hpp"
#include "symbology.hpp"
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

namespace {

  const char * const fix_type_strings[] = {
       "no gps"
      ,"no fix" 
      ,"2D fix"
      ,"3D fix"
      ,"3D fix (dgps)"
      ,"3D fix (rtk)"
  };       
}

void draw_gps_state()
{
   font_ptr font = get_font(FontID::OSD_Charset);

   pxp_type pos;
   char buf[30];

   if ( osd_show_gps_fix_type() == true)
   {
	   pos = get_osd_gps_fix_type_position();

	   if (font)
	   {
		  uint8_t const fix_type = read_gps_fix_type();
		  if ( fix_type < 5){
			 sprintf(buf,"%s", fix_type_strings[fix_type]);
			 draw_text(buf,pos,font);
		  }
	   }
	}

   if ( osd_show_gps_num_sats() == true)
   {
	   pos = get_osd_gps_num_sats_position();
	   if (font)
	   {
		 sprintf(buf,"%d", static_cast<int>(read_gps_num_sats()));
		 draw_text(buf,pos,font);
	   }
   }

   if ( osd_show_gps_hdop() == true)
   {
	   pos = get_osd_gps_hdop_position();

	   if (font)
	   {
		  quan::length_<uint16_t>::cm cur_hdop;
		  uint16_t display_hdop;

		  display_hdop = cur_hdop.numeric_value();

		  quan::constrain<uint16_t> (
				  display_hdop
				, 0
				, 999);

		  sprintf(buf,"%3d", display_hdop);
		  draw_text(buf,pos,font);
	   }
   }
}
