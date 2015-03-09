#include <cstdint>
#include <cstdio>
#include <quan/uav/osd/api.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/osd/features_api.hpp>
//#include <quan/uav/osd/home_position_is_set.hpp>
#include "osd.hpp"
#include "symbology_config.hpp"

using namespace quan::uav::osd;
using quan::length;

// todo add a home_position_is_set function

void draw_altitude()
{
   pxp_type pos;
   position_type const aircraft_position = get_aircraft_position();
   position_type const home_position = get_home_position();
   length::m alt;

   int alt_m;
   size_type vect;
   bitmap_ptr bitmap ;
   font_ptr font ;
   char buf[30];

#if ((ALTITUDE_ENABLE & ENABLE_ON_PAGE_ALL) != 0)
   alt = aircraft_position.alt - home_position.alt;
   pos =
        {ALTITUDE_X,
        (( get_video_mode() == video_mode::pal)
        ?ALTITUDE_Y_PAL:ALTITUDE_Y_NTSC)};

   vect= {0,0};

   bitmap = get_bitmap(BitmapID::altitude_symbol);
   if (bitmap) {
       vect = get_size(bitmap) / 2;
       draw_bitmap(bitmap,pos);
   }
   alt_m = static_cast<int> (alt.numeric_value() + 0.5f);

   font = get_font(FontID::OSD_Charset);
   if ( font){
      sprintf( buf,"%4d%c",alt_m,141);
      draw_text(buf,pos + pxp_type{vect.x,0},font);
   }
#endif


#if ((BARO_ALTITUDE_ENABLE & ENABLE_ON_PAGE_ALL) != 0)

   alt = get_baro_alt();

   pos =
        {BARO_ALTITUDE_X,
        (( get_video_mode() == video_mode::pal)
        ?BARO_ALTITUDE_Y_PAL:BARO_ALTITUDE_Y_NTSC)};

   vect= {0,0};

   bitmap = get_bitmap(BitmapID::altitude_symbol);
   if (bitmap) {
       vect = get_size(bitmap) / 2;
       draw_bitmap(bitmap,pos);
   }

   alt_m = static_cast<int> (alt.numeric_value() + 0.5f);

   font = get_font(FontID::OSD_Charset);
   if ( font){
      sprintf( buf,"%4d%c",alt_m,141);
      draw_text(buf,pos + pxp_type{vect.x,0},font);
   }
#endif

}

