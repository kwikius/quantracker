
#include <cmath>
#include <cstring>

#include <quan/angle.hpp>
#include <quan/two_d/rotation.hpp>
#include <quan/dynarray.hpp>

#include "video_cfg.hpp"
#include "graphics_api.hpp"
#include <quan/stm32/push_pop_fp.hpp>

/*
 need to yield at times to do other tasks,
 ideally once per millisec
 alternately use irq to do tasks?
 In systick reeenable interrupts
 Or get a big buffer for input and do other tasks at the end.
 1/50th sec at 56 k baud
 5,600 bytes/sec
 = 5,600 / 50
 = requires 112 bytes of buffer per 1/50th sec frame
 also need to process.
*/
namespace {

   typedef quan::two_d::vect<float> vect;
   vect pos {0.f,0.f};
   vect inc {1.f,1.f};
   float sq = 150.f;
   quan::angle_<float>::deg cur_rotation{0.f};
   quan::two_d::vect<float> bx0{-sq, sq};
   quan::two_d::vect<float> bx1{ sq, sq};
   quan::two_d::vect<float> bx2{sq, -sq};
   quan::two_d::vect<float> bx3{-sq, -sq};

// if receiver will be filled from telem
// think its 117 bytes/half_frame
// make sure this is big enough!
   char text_data[200] = {0};

};

void set_text_data( const char* text)
{
   strcpy(text_data, text);
}

void on_draw()
{
  // quan::stm32::push_FPregs();
   vect display_size = video_cfg::get_display_size_px();

   quan::two_d::rotation rotate{cur_rotation};
   quan::two_d::vect<float> center = display_size/2.f;

   for ( int32_t i = 0; i < 10; ++i){
      color col = ((i > 2)&& (i < 8))?color::white:color::black;
      draw_line_bres ( rotate(bx0 + vect{-i,i})+ center, rotate(bx1 + vect{i,i})+ center,col);
      draw_line_bres ( rotate(bx1 + vect{i,i})+ center, rotate(bx2+vect{i,-i})+ center,col);
      draw_line_bres ( rotate(bx2 + vect{i,-i})+ center, rotate(bx3+vect{-i,-i})+ center,col);
      draw_line_bres ( rotate(bx3 +vect{-i,-i})+ center, rotate(bx0 + vect{-i,i})+ center,col);
   }

   cur_rotation += quan::angle_<float>::deg{60.f}/ 50.f;

   if(cur_rotation >= quan::angle_<float>::deg{360.f}){
      cur_rotation -= quan::angle_<float>::deg{360.f};
   }

   draw_line_bres ( 
      {0,0},
      {display_size.x-1.f,0.f},color::black);

   draw_line_bres (
      {display_size.x-1.f,0.f},
      {display_size.x-1.f,display_size.y-1.f},color::black);
   
   draw_line_bres (
      {0, display_size.y-1.f},
      {display_size.x-1.f,display_size.y-1.f},color::black);
   draw_line_bres ( 
      {0,0},
      {0,display_size.y-1},color::black);
   
   pos += inc;
   if (pos.x > display_size.x/2) {
      inc.x = -1;
   } else {
      if (pos.x < -display_size.x/2) {
         inc.x = 1;
      }
   }
   if (pos.y > display_size.y/2) {
      inc.y = -1;
   } else {
      if (pos.y < -display_size.y/2) {
         inc.y = 1;
      }
   }
   
   //const char* str = "www.zoomworks.org";
   //vect2 get_fonted_dims(str)
   vect text_dims {0,0};
   for (const char* ptr = text_data; *ptr != '\0'; ++ptr) {
      abc_bitmap<uint8_t>* fontch = get_font_char (*ptr);
      if (fontch) {
         text_dims.x += fontch->get_size_px().x;
         int32_t y = fontch->get_size_px().y;
         if (y > text_dims.y) {
            text_dims.y = y;
         }
      }
   }
   draw_text(display_size/2 - text_dims/2 + pos,text_data);

   vect const border {5,5};
   
   quan::two_d::vect<float> p0 {- (text_dims.x/2 + border.x), - (text_dims.y/2 + border.y) };
   quan::two_d::vect<float> p1 { (text_dims.x/2 + border.x),- (text_dims.y/2 + border.y) };
   quan::two_d::vect<float> p2 { (text_dims.x/2 + border.x), (text_dims.y/2 + border.y) };
   quan::two_d::vect<float> p3 {- (text_dims.x/2 + border.x), (text_dims.y/2 + border.y) };
   
   vect const offset = display_size/2 + pos;
   
   p0 += offset;
   p1 += offset;
   p2 += offset;
   p3 += offset;
   
   draw_line_bres (p0,p1,color::black);
   draw_line_bres (p1,p2,color::black);
   draw_line_bres (p3,p2,color::black);
   draw_line_bres (p0,p3,color::black);

   p0 += vect {-1,-1};
   p1 += vect {1,-1};
   p2 += vect {1,1};
   p3 += vect {-1,1};
   draw_line_bres (p0,p1,color::black);
   draw_line_bres (p1,p2,color::black);
   draw_line_bres (p3,p2,color::black);
   draw_line_bres (p0,p3,color::black);
   
   p0 += vect {-1,-1};
   p1 += vect {1,-1};
   p2 += vect {1,1};
   p3 += vect {-1,1};
   
   draw_line_bres (p0,p1,color::white);
   draw_line_bres (p1,p2,color::white);
   draw_line_bres (p3,p2,color::white);
   draw_line_bres (p0,p3,color::white);

   p0 += vect {-1,-1};
   p1 += vect {1,-1};
   p2 += vect {1,1};
   p3 += vect {-1,1};
   
   draw_line_bres (p0,p1,color::white);
   draw_line_bres (p1,p2,color::white);
   draw_line_bres (p3,p2,color::white);
   draw_line_bres (p0,p3,color::white);

}
 