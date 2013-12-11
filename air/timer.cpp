

#include <stm32f0xx.h>

#include "events.hpp"

namespace {
    uint32_t tickcount;
}

uint32_t millis(){ return tickcount;}

quan::event ms20_event;

extern "C" void SysTick_Handler()
{
   ++tickcount;
   static uint32_t count20ms = 0;
   if( ( count20ms = (count20ms + 1 ) % 20 ) == 0){
      ms20_event.set();
   }   
}