#include <cstdio>

#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/get_distance.hpp>
#include "osd.hpp"

using namespace quan::uav::osd;

void draw_home()
{
   size_type vect;
   pxp_type pos{-160,70};
   bitmap_ptr home_image = get_bitmap(BitmapID::home_image);
   if (home_image) {
      vect = get_size(home_image) / 2;
      draw_bitmap (home_image,pos);
      auto const & aircraft_position = get_aircraft_position();
      auto const & home_position = get_home_position();
      auto const & distance = quan::uav::get_distance(aircraft_position,home_position);
      auto const d_m = static_cast<uint32_t> (distance.numeric_value() + 0.5f);
      char buf[30];
      sprintf( buf,"%5u M",d_m);
      font_ptr font = get_font(FontID::OSD_Charset);
      if ( font){
         draw_text(buf,pos + pxp_type{vect.x,0},font);
      }
   }
}