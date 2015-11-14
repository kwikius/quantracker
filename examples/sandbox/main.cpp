/*
Minimal Quantracker Air OSD Hello World application
*/

#include <quantracker/osd/osd.hpp>

int main()
{
   osd_setup();           
   create_draw_task();    
   vTaskStartScheduler(); 
}
// test set get pixel
void quan::uav::osd::on_draw()
{

   pxp_type pos{50,50};
   draw_circle(10,pos,color_type::black);

   flood_fill(pos,color_type::black,400 );

   pos = pxp_type{-50,50};
   draw_circle(10,pos,color_type::white);
   draw_box(pos + pxp_type{-5,-5},pos + pxp_type{5,5},color_type::white, false);

   flood_fill(pos + pxp_type{-7,0},color_type::white,400 );
  
}
