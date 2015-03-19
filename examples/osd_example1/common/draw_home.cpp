#include <cstdio>
#include <quan/uav/osd/features_api.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/get_distance.hpp>
#include "osd.hpp"
#include "symbology.hpp"

using namespace quan::uav::osd;

void draw_home()
{
   bitmap_ptr home_image = get_bitmap(BitmapID::home_image);
   if (home_image) {
      pxp_type pos = get_display_home_position();
      size_type vect = get_size(home_image) / 2;
      draw_bitmap (home_image,pos);
      char buf[30];
      font_ptr font = get_font(FontID::OSD_Charset);
      if ( font){
         if (home_position_is_set()){
            auto const  aircraft_position = get_aircraft_position();
            auto const  home_position = get_home_position();
            quan::length::m const distance 
               = quan::uav::get_distance<quan::length::m>(aircraft_position,home_position);
            int const d_m = static_cast<int> (distance.numeric_value() + 0.5f);
            sprintf( buf,"%5dm",d_m);
         }else{
            sprintf ( buf,"~~~~~~");
         }
         draw_text(buf,pos + pxp_type{vect.x,0},font);
      }
   }
}