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

// your custom drawing function
void quan::uav::osd::on_draw()
{
   draw_text("Hello World",{-60,0});
}
