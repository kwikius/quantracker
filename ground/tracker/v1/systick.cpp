
#include "resources.hpp"
#include "events.hpp"
#include <quan/stm32/systick.hpp>

volatile int64_t quan::stm32::detail::systick_tick::current = 0;

void setup_systick()
{
     // configure systeick for 1 ms;
#if __Vendor_SysTickConfig == 1
#error "need to define this"
#endif
  SysTick_Config(SystemCoreClock / 1000);
  NVIC_SetPriority(SysTick_IRQn,interrupt_priority::systick_timer);
}

extern "C" void Systick_Handler() __attribute__ ((interrupt ("IRQ")));
extern "C" void SysTick_Handler()
{
   ++quan::stm32::detail::systick_tick::current;
   ms1_event.set();
}

