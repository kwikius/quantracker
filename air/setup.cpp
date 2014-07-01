
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

#include "serial_ports.hpp"
#include "fsk.hpp"
#include "frsky.hpp"
#include "events.hpp"

extern "C" void __cxa_pure_virtual() { while (1); }
void *__dso_handle;

void operator delete (void*p){ ;}

void setup_outputs()
{

   frsky::setup();
   fsk::setup();
}

void setup_events()
{
   frsky::setup_event();
   fsk::setup_event();
   setup_heartbeat_event();

   SysTick_Config(SystemCoreClock / 1000);
}

void setup_inputs()
{
   posdata_sp::serial_port::init();
//TODO : invert tx output due to transistor inverter on output
   // N.B  for mavlink only
   // for GPS depends on config
   posdata_sp::serial_port::set_baudrate<57600,false>();
}

extern "C" void setup()
{
  // first check if user wants to dialog
  // if not then ..
  // read config
  setup_outputs();
  setup_events();
  setup_inputs(); 
}

