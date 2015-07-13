
/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korhonen(Openpilot.org)
   taulabs ( taulabs.com) 
   brainFPV ( brainfpv.com)
   Thomas Oldbury (super-osd.com)

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

#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/usart/irq_handler.hpp>
#include <quan/stm32/gpio.hpp>
#include <quantracker/osd/osd.hpp>

#include "resources.hpp"
#include "fsk.hpp"
#include "frsky.hpp"

extern "C" void setup()
{
   osd_setup();
   fsk::setup();
   mavlink_tx_rx_task::setup<57600>(interrupt_priority::telemetry_input_port);
   frsky_tx_rx_task::setup<9600>(interrupt_priority::frsky_serial_port);
}
