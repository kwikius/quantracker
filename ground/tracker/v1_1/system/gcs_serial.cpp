
#include <type_traits>
#include "resources.hpp"

extern "C" void USART1_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART1_IRQHandler()

{
   static_assert(
   std::is_same<
     gcs_serial_usart,quan::stm32::usart1
   >::value ,"invalid usart for serial_port irq");
   gcs_serial::irq_handler();
}