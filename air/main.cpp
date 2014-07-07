
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
#include <cstdint>

#include "mavlink.hpp"
#include "gps.hpp"
#include "settings.hpp"
#include "events.hpp"
#include "resources.hpp"

extern "C" void setup();

namespace {
//
//   void read_gps()
//   { 
//      the_gps.parse();
//   }

   void read_data()
   {
//      switch( settings::data_source){
//         case settings::data_source_t::mavlink:
            read_mavlink();
//         break;
//         default:
//            read_gps();
//         break;
//      }
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
int main()
{
   // for user input CLI
   // setup the sp at baud
   // listen for user data for 30 secs
   // if first three bytes == rets then command line else
   //
  // if (! want_commandline() ){
      setup();
      for(;;){
         read_data();
       
        service_events();
      }
//   }else{
//      do_command_line();
//   }
}
