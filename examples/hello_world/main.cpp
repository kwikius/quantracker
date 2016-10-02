/*
Minimal Quantracker Air OSD Hello World application
*/

#include <quan/stm32/gpio.hpp>
#include <quantracker/osd/osd.hpp>
#include "../../air/osd/resources.hpp"
int main()
{
   osd_setup();           
   create_draw_task();   
   quan::stm32::set<heartbeat_led_pin>(); 
#if defined QUAN_AERFLITE_BOARD
    quan::stm32::set<notify_led1>(); 
    quan::stm32::set<notify_led2>(); 
#endif
   vTaskStartScheduler(); 
}

// your custom drawing function
void quan::uav::osd::on_draw()
{
   //draw_text("Hello World",{-60,0}); // nothing
  //  draw_line ( {-100,-100}, {100,100}, colour_type::black );
    draw_line ( {-120,-100}, {80,100}, colour_type::white ); // white
    draw_line ( {-100,-100}, {100,100}, colour_type::black ); // black
  //  draw_line ( {-115,-100}, {85,100}, colour_type::transparent );
}
