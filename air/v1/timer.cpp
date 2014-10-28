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


#include <stm32f0xx.h>

#include "events.hpp"

namespace {
    uint32_t tickcount;
}

quan::time_<uint32_t>::ms millis()
{ return quan::time_<uint32_t>::ms{tickcount};}

extern "C" void SysTick_Handler()
{
   ++tickcount;
   
   do_event_ticks(); 
}
