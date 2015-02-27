
#include <quan/uav/osd/api.hpp>
#include <quan/two_d/rotation.hpp>
#include <quan/uav/osd/get_aircraft_attitude.hpp>
#include "osd.hpp"

using namespace quan::uav::osd;

namespace{
   struct rotation {

      rotation(quan::angle_<float>::deg const & pitch, quan::angle_<float>::deg const & roll)
      :cos_roll{cos(roll)}
      ,sin_roll{sin(roll)}
      ,cos_pitch{cos(pitch)}
      ,sin_pitch{sin(pitch)}{}

      quan::two_d::vect<float> operator()(quan::two_d::vect<float> const & in)const
      {
         // represents the distance of the screen from the viewers eye.
         // prob calc from viewing angle
         // where screen is z units wide then can work from there
         float z = 400;
         quan::two_d::vect<float> result{
            in.x * cos_roll - in.y  * sin_roll * cos_pitch + z * sin_roll * sin_pitch
            ,in.x * sin_roll + in.y * cos_roll * cos_pitch - z * cos_roll * sin_pitch
         };
#if 1
         float z1 = in.y * sin_pitch + z * cos_pitch;
         if ( z1 < 0.5){
            z1 = 0.5;
         }
         float const ratio = z / z1;
         result *= ratio;
#endif
         return result;
      }
     private:
      float cos_roll;
      float sin_roll;
      float cos_pitch;
      float sin_pitch;
   };
}

/*
   need to prevent drawing if round the back
*/


void draw_artificial_horizon()
{
   constexpr int32_t width = 127;
   constexpr int32_t outer_h_bar_len = 16;
   constexpr int32_t outer_stop_height = 8;
   constexpr int32_t centre_rad = 5;

   pxp_type left_end{-width/2,0};
   pxp_type right_end{width/2,0};

   auto const & attitude = get_aircraft_attitude();
   rotation rotate{attitude.pitch,attitude.roll};

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
      draw_circle(centre_rad+i,rotate({0,0}),c);
      draw_line(rotate(pxp_type{-20,i}),rotate(pxp_type{-centre_rad - 1 ,i}), c);
      draw_line(rotate(pxp_type{20,i}),rotate(pxp_type{centre_rad + 1,i}), c); 
   }
}
