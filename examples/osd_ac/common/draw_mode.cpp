
#include <cstdio>
#include <cstring>
#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "on_draw.hpp"
#include "symbology.hpp"
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;

namespace {

  const char * const mode_strings[] = {
        "stab" //Stabilize
		,"acro" //Acrobatic
		,"alth" //Alt Hold
		,"auto" //Auto
		,"guid" //Guided
		,"loit" //Loiter
		,"rtl " //Return to Launch
		,"circ" //Circle
		,"md8"  //Position Hold (Old)
		,"land" //Land
		,"oflo" //OF_Loiter
		,"drif" //Drift
		,"md12" //Position Hold (Old)
		,"sprt" //Sport
		,"flip" //Flip
		,"tune" //Tune
		,"phld" //Position Hold (Earlier called Hybrid)
  };       
}

void draw_mode()
{
   font_ptr font = get_font(FontID::OSD_Charset);

   pxp_type pos;
   char buf[30];

	if ( osd_show_flight_mode() == true)
	{
	   pos = get_osd_flight_mode_position();

	   if (font){

		  uint8_t const mode = get_custom_mode();

		  if ( mode < 17){
			 sprintf(buf,"%s", mode_strings[mode]);
		  }

		  draw_text(buf,pos,font);
	   }
	}


	if ( osd_show_armed_mode() == true)
	{
	   pos = get_osd_armed_mode_position();

	   if (font){

		  uint8_t const mode = get_base_mode();

		  if ((mode & 0x80)!=0){
			 sprintf(buf,"%s", "armed");
		  }
		  else {
			 sprintf(buf,"%s", "no-arm");
		  }

		  draw_text(buf,pos,font);
	   }
	}
}
