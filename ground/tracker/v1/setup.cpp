
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

#include "main_loop.hpp"
#include "azimuth.hpp"
#include "serial_ports.hpp"
#include "leds.hpp"
#include "switch_input.hpp"
#include "compass.hpp"

void setup_systick();

// blink error led forever???
extern "C" void __cxa_pure_virtual() { while (1); }
void *__dso_handle;

// till we have op new...
void operator delete (void*p){ ;}

extern "C" void setup()
{
  
   setup_leds();
   setup_switches();
   setup_systick();
   main_loop::elevation_servo_setup_ports();
   azimuth::motor::setup_ports();

   frsky::serial_port::set_irq_priority(interrupt_priority::frsky_serial_port);
   rctx::serial_port::set_irq_priority(interrupt_priority::rctx_serial_port);

   frsky::serial_port::init();

   rctx::serial_port::init();

   azimuth::encoder::setup();
   setup_compass();
   main_loop::setup();

}