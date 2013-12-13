
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
#include "dac.hpp"
#include "resources.hpp"

extern "C" void __cxa_pure_virtual() { while (1); }
void *__dso_handle;

void operator delete (void*p){ ;}

void dac_setup();

using namespace quan::stm32;

extern "C" void setup()
{

  SysTick_Config(SystemCoreClock / 1000);

  module_enable<heartbeat_led_pin::port_type>();

   apply<
      heartbeat_led_pin
      , gpio::mode::output
      , gpio::otype::push_pull
      , gpio::pupd::none
      , gpio::ospeed::slow
      , gpio::ostate::high
   >();

//   set dac priority ( highest)
//   posdata::serial_port::set_irq_priority(interrupt_priority::pos_data_serial_port); (2nd highest)
//   frsky::serial_port::set_irq_priority(interrupt_priority::frsky_serial_port); 
// TODO check for settings input via sp

   frsky_sp::serial_port::init();
//TODO : invert tx output according to eeprom
   frsky_sp::serial_port::set_baudrate<9600,false>();

   posdata_sp::serial_port::init();
//TODO : invert tx output due to transistor inverter on output
   posdata_sp::serial_port::set_baudrate<57600,false>();

   dac_setup();
   
}

