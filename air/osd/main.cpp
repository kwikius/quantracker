
/*
 Copyright (c) 2013 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#ifndef QUAN_OSD_BOARD_TYPE
   #error which board?
#else
   #if QUAN_OSD_BOARD_TYPE == 1
      #pragma message "for board 1"
   #else 
      #if QUAN_OSD_BOARD_TYPE == 2
         #pragma message "for board 2"
      #else
         #if QUAN_OSD_BOARD_TYPE == 3
            #pragma message "for board 3"
         #else
            #error unknown board
         #endif
      #endif
   #endif
#endif

#include <cstdint>
#include "mavlink.hpp"
#include "gps.hpp"
#include "settings.hpp"
#include "events.hpp"
#include "resources.hpp"

extern "C" void setup();

namespace {

   void read_data()
   {
      read_mavlink();
   }
}
#if 0
namespace {
    /*
    Set TXO pin as input with pullup. Then read it.
    if it reads low then want command line mode return true
    or could change to three rets after period of nothing ..like APM?
   */
   bool want_commandline()
   {
      quan::stm32::module_enable<posdata_txo_pin::port_type>();
      quan::stm32::apply<
         posdata_txo_pin,
         quan::stm32::gpio::mode::input,
         quan::stm32::gpio::pupd::pull_up
      >();
      // delay so pullup can act.
      for ( uint32_t i =0; i < 200; ++i){
         asm("nop");
      }
      bool result = quan::stm32::get<posdata_txo_pin>();
      //  put things back
       quan::stm32::apply<
         posdata_txo_pin,
         quan::stm32::gpio::pupd::none
      >();
      quan::stm32::module_disable<posdata_txo_pin::port_type>();
      return result;
   }

   void do_command_line(){}

}
#endif

void do_tasks()
{
   read_data();
   service_events();
}
int main()
{
   setup();
   for(;;){
      draw_loop();
      do_tasks();
   }
}
