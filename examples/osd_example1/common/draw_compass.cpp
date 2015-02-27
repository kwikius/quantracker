
#include <quan/uav/osd/api.hpp>
#include <quan/two_d/rotation.hpp>
#include <quan/uav/osd/get_aircraft_heading.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/get_home_position.hpp>
#include <quan/uav/get_bearing.hpp>
#include "osd.hpp"

using namespace quan::uav::osd;

void draw_compass () 
{
   angle_type const heading = get_aircraft_heading();
   auto const aircraft_pos = get_aircraft_position();
   auto const home_pos = get_home_position();
   angle_type const home_bearing 
      = quan::uav::get_bearing(aircraft_pos,home_pos);
   
   quan::two_d::rotation const rotate {heading}; 
   pxp_type const pos = 
      {0,
      (( get_video_mode() == video_mode::pal)
      ?-115:-65)};
   int constexpr radius = 25;
   draw_circle(radius + 2,pos, colour_type::black);
   // should prob make this a bitmap for speed
   // since its constant
   for (int i = 0; i < 16; ++i) {
      constexpr auto offset = angle_type {360.f / 32.f};
      angle_type const basic_angle = angle_type { (360.f * i) / 16.f};
      angle_type const start_angle = basic_angle - offset;
      angle_type const end_angle = basic_angle + offset;
      colour_type col 
         = (i & 1) 
         ? colour_type::white 
         : colour_type::black;
      draw_arc (radius + 1,pos, start_angle, end_angle,  col);
      draw_arc (radius,pos, start_angle, end_angle,  col);
   }
   draw_circle (radius - 1, pos,  colour_type::black);

   bitmap_ptr home_arrow = get_bitmap(BitmapID::home_arrow);
   if (home_arrow) {
      size_type const vect = get_size(home_arrow) / 2;
      draw_bitmap(home_arrow, pos, vect, heading - home_bearing);
   }

   bitmap_ptr compass_arrow = get_bitmap(BitmapID::compass_arrow);
   if (compass_arrow) {
      size_type const vect = get_size(compass_arrow) / 2;
      draw_bitmap (compass_arrow,pos,vect,heading);
   }
   font_ptr font = get_font(FontID::MWOSD);
   if (font) {
      size_type const char_size = get_size(font );
      pxp_type const  char_offset = - char_size / 2;
      constexpr auto font_radius = 19;
      constexpr char compass_char[] {'N', 'S', 'E', 'W'};
      constexpr pxp_type compass_vector[] {
         {0, font_radius}
         , {0, - font_radius}
         , {font_radius, 0}
         , { -font_radius, 0}
      };
      for (size_t i = 0; i < 4; ++i) {
         bitmap_ptr char_bmp = get_char(font,compass_char[i]);
         if (char_bmp) {
            auto const char_pos = rotate (compass_vector[i]);
            pxp_type const char_pos_i = pos + char_pos + char_offset ;
            draw_bitmap (char_bmp,char_pos_i);
         }
      }
   }
}
