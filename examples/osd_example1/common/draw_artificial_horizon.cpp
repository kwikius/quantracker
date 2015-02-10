
#include <quan/uav/osd/api.hpp>
#include <quan/two_d/rotation.hpp>
#include <quan/uav/osd/get_aircraft_attitude.hpp>
#include "osd.hpp"

using namespace quan::uav::osd;

void draw_artificial_horizon()
{
   constexpr int32_t width = 127;
   constexpr int32_t outer_h_bar_len = 16;
   constexpr int32_t outer_stop_height = 8;
   constexpr int32_t centre_rad = 5;

   pxp_type left_end{-width/2,0};
   pxp_type right_end{width/2,0};

   quan::two_d::rotation rotate{get_aircraft_attitude().roll};

   for (int32_t i = -1; i < 2; ++i){
      const colour_type c = (i)?colour_type::black:colour_type::white;
      draw_line(
         rotate(left_end + pxp_type{1,i}), 
         rotate(left_end + pxp_type{outer_h_bar_len ,i}), c
      );
      draw_line(
         rotate(right_end+ pxp_type{0,i}), 
         rotate(right_end + pxp_type{-outer_h_bar_len,i}),c
      );
      draw_line(
         rotate(pxp_type{left_end.x - i,outer_stop_height/2}),
         rotate(pxp_type{left_end.x - i,-outer_stop_height/2}),c
      );
      draw_line(
         rotate(pxp_type{right_end.x +i,outer_stop_height/2}),
         rotate(pxp_type{right_end.x +i,-outer_stop_height/2}),c
      );
      draw_circle(centre_rad+i,{0,0},c);
      draw_line(rotate(pxp_type{-20,i}),rotate(pxp_type{-centre_rad - 1 ,i}), c);
      draw_line(rotate(pxp_type{20,i}),rotate(pxp_type{centre_rad + 1,i}), c); 
   }
}
