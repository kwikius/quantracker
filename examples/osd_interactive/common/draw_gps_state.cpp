
#include <cstdio>
#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "on_draw.hpp"
#include "osd.hpp"

using namespace quan::uav::osd;

namespace {

  const char * const fix_type_strings[] = {
       "no GPS"
      ,"no fix" 
      ,"2D fix"
      ,"3D fix"
      ,"3D fix (DGPS)"
      ,"3D fix (RTK)"
  };       
}

void draw_gps_state()
{
   font_ptr font = get_font(FontID::OSD_Charset);
   if (font){
      size_type font_size = get_size(font);
      pxp_type pos{-160,70};
      char buf[30];
      uint8_t const fix_type = read_gps_fix_type();
      if ( fix_type < 5){
         sprintf(buf,"fix type: %s", fix_type_strings[fix_type]);
         draw_text(buf,pos,font);
         pos.y += font_size.y + 2;
      }
      sprintf(buf,"num sats: %3d", static_cast<int>(read_gps_num_sats()));
      draw_text(buf,pos,font);
   }
}