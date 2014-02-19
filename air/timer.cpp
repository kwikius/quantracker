

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