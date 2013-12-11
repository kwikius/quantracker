
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
#include "resources.hpp"

//#include "timer.hpp"
//#include "led.hpp"
#include "mavlink.hpp"
#include "frsky.hpp"
#include "serial_ports.hpp"

//#include "serial_ports.hpp"
#include "dac.hpp"
#include "events.hpp"

#include "gps.hpp"

#if 0 
/* change BOOT1 option to allow use of serial port to program.
If one is written then all must be rewritten
*/
#if defined QUAN_STM32F0
extern uint32_t const user_options1 __attribute__ ((section ("processor_options1")))  = 0x10EF55AA;
extern uint32_t const user_options2 __attribute__ ((section ("processor_options2")))  = 0x00FF00FF;
extern uint32_t const user_options3 __attribute__ ((section ("processor_options3")))  = 0x00FF00FF;
#endif
#endif
//todo

namespace {
  void read_gps()
{ 
      the_gps.parse();
}


   void update_leds()
   {
      static uint32_t heartbeat_timer = 0;
      static uint32_t cur_num_heartbeats = 0;
      static bool heartbeat_led_on = false;
      uint32_t num_heartbeats = get_num_heartbeats();
      if ( num_heartbeats > cur_num_heartbeats){
         // new heartbeat
         cur_num_heartbeats = num_heartbeats;
         heartbeat_timer = millis();
         quan::stm32::set<heartbeat_led_pin>();
         heartbeat_led_on = true;
      }else{
         //turn off heartbeat led after a one shot pulse of 1/4 sec 
         if ( heartbeat_led_on && (( millis() - heartbeat_timer ) >= 250)){
           quan::stm32::clear<heartbeat_led_pin>();
           heartbeat_led_on = false;
         }
      }
   }
//true if want to get mavllink data
   bool want_mavlink = true;
   void read_data()
   {
      if (want_mavlink){
         read_mavlink();
      }else{
         read_gps();
      }
   }
}


int main()
{
   for(;;){
      read_data();
      if( ms20_event.signalled()){
         ms20_event.clear();
         update_leds();
         FrSky_send_message();
      }
   }
}
