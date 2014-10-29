/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */

#include <quan/stm32/gpio.hpp>

#include "heartbeat.hpp"
#include "resources.hpp"
#include "events.hpp"
#include "mavlink.hpp"

/*
  Once the heartbeat is done, switch off led and reversed to waiting for the next
*/
namespace {
   periodic_event heartbeat_event{quan::time_<uint32_t>::ms{10U},on_new_heartbeat,true};
}

using namespace quan::stm32;

void on_heartbeat_done()
{
   clear<heartbeat_led_pin>();
   heartbeat_event.set(quan::time_<uint32_t>::ms{10U},on_new_heartbeat);
}

/*
On new heartbeat Switch on LED and change function to counting in heartbeat
*/

void on_new_heartbeat()
{
   static uint32_t cur_num_heartbeats = 0;

   uint32_t num_heartbeats = get_num_heartbeats();
   if ( num_heartbeats > cur_num_heartbeats){
      // new heartbeat
      cur_num_heartbeats = num_heartbeats;
      set<heartbeat_led_pin>();
      heartbeat_event.set(quan::time_<uint32_t>::ms{250U},on_heartbeat_done);
   }
}

void setup_heartbeat_event()
{
   module_enable<heartbeat_led_pin::port_type>();

   apply<
      heartbeat_led_pin
      , gpio::mode::output
      , gpio::otype::push_pull
      , gpio::pupd::none
      , gpio::ospeed::slow
      , gpio::ostate::low
   >();

  set_event(event_index::heartbeat,&heartbeat_event);
}
