/*
Minimal Quantracker Air OSD Hello World application
*/

#include <quan/stm32/gpio.hpp>
#include <quantracker/osd/osd.hpp>
#include <quantracker/air/osd/resources.hpp>
int main()
{
   osd_setup();           
   create_draw_task();   
   quan::stm32::set<heartbeat_led_pin>(); 
   vTaskStartScheduler(); 
}

// your custom drawing function
void quan::uav::osd::on_draw()
{
   draw_text("Hello World",{-60,0});
}
