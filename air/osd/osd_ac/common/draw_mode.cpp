
#include <cstdio>
#include <cstring>
#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "on_draw.hpp"
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

#if ((FLIGHT_MODE_ENABLE & ENABLE_ON_PAGE_ALL) != 0)

   pos = {FLIGHT_MODE_X,
          (( get_video_mode() == video_mode::pal)
          ?FLIGHT_MODE_Y_PAL:FLIGHT_MODE_Y_NTSC)};


   if (font){

      uint8_t const mode = get_custom_mode();

      if ( mode < 17){
         sprintf(buf,"%s", mode_strings[mode]);
      }

      draw_text(buf,pos,font);
   }
#endif


#if ((ARMED_MODE_ENABLE & ENABLE_ON_PAGE_ALL) != 0)

   pos = {ARMED_MODE_X,
          (( get_video_mode() == video_mode::pal)
          ?ARMED_MODE_Y_PAL:ARMED_MODE_Y_NTSC)};


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
#endif

}
