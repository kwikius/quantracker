#include <cstdint>
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
//#include <quan/uav/osd/home_position_is_set.hpp>
#include "osd.hpp"

using namespace quan::uav::osd;
using quan::length;

// todo add a home_position_is_set function

void draw_altitude()
{
   position_type const & aircraft_position = get_aircraft_position();
   position_type const & home_position = get_home_position();
   length::m const alt = aircraft_position.alt - home_position.alt;

   size_type vect= {0,0};
   pxp_type const pos{-160,100}; 
   bitmap_ptr bitmap = get_bitmap(BitmapID::altitude_symbol);
   if (bitmap) {
       vect = get_size(bitmap) / 2;
       draw_bitmap(bitmap,pos);
   }
   int const alt_m = static_cast<int> (alt.numeric_value() + 0.5f);

   font_ptr font = get_font(FontID::OSD_Charset);
   if ( font){
      char buf[30];
      sprintf( buf,"%5dm",alt_m);
      draw_text(buf,pos + pxp_type{vect.x,0},font);
   }
}

