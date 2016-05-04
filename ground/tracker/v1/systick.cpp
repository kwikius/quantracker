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

#include "resources.hpp"
#include "events.hpp"
#include <quan/stm32/systick.hpp>

void setup_systick()
{
     // configure systeick for 1 ms;
#if __Vendor_SysTickConfig == 1
#error "need to define this"
#endif
  SysTick_Config(SystemCoreClock / 1000);
  NVIC_SetPriority(SysTick_IRQn,15);
}

#if 1
quan::stm32::detail::systick_tick::current_t 
   quan::stm32::detail::systick_tick::current = {0,0};

extern "C" void Systick_Handler() __attribute__ ((interrupt ("IRQ")));
extern "C" void SysTick_Handler()
{
   if ( quan::stm32::detail::systick_tick::current.ar[0] == 0xffffffff){
       quan::stm32::detail::systick_tick::current.ar[0] = 0;
       ++ quan::stm32::detail::systick_tick::current.ar[1];
   }else{
       ++ quan::stm32::detail::systick_tick::current.ar[0];
   }
}

#else
volatile int64_t quan::stm32::detail::systick_tick::current = 0;

extern "C" void Systick_Handler() __attribute__ ((interrupt ("IRQ")));
extern "C" void SysTick_Handler()
{
   ++quan::stm32::detail::systick_tick::current;
   ms1_event.set();
}
#endif

