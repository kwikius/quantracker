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
// not needed with RTOS
#if 0
#include "events.hpp"

#include <quan/stm32/systick.hpp>

volatile int64_t quan::stm32::detail::systick_tick::current = 0;

extern "C" void Systick_Handler() __attribute__ ((interrupt ("IRQ")));
extern "C" void SysTick_Handler()
{
   ++quan::stm32::detail::systick_tick::current;
   do_event_ticks(); 
}
#endif

   
   
