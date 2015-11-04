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

   draw_circle(10,{50,50},color_type::black);

   flood_fill({50,50},color_type::black,400 );

  
}
