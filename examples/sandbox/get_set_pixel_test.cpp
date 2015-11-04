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
// test set/get pixel
void quan::uav::osd::on_draw()
{

   pxp_type pos{-120,120};

   set_pixel({20,20},color_type::grey);
   if ( get_pixel({20,20}) == color_type::grey){
      draw_text("success",pos);
   }else{
      draw_text("fail",pos);
   }
   pos.y -= 20;
   if ( get_pixel({20,20}) == color_type::black){
      draw_text("fail1",pos);
   }else{
      draw_text("success1",pos);
   }
   pos.y-= 20;
   set_pixel({21,21},color_type::black);
   if ( get_pixel({21,21}) == color_type::black){
      draw_text("success2",pos);
   }else{
      draw_text("fail2",pos);
   }
   pos.y-= 20;
   set_pixel({22,22},color_type::white);
   if ( get_pixel({22,22}) == color_type::white){
      draw_text("success3",pos);
   }else{
      draw_text("fail3",pos);
   }
   pos.y-= 20;
   // n.b think that setting pixel transparent does nothing
   // may need to redo to foce transparent
   set_pixel({23,23},color_type::transparent);
   if ( get_pixel({23,23}) == color_type::transparent){
      draw_text("success4",pos);
   }else{
      draw_text("fail4",pos);
   }
   
}
