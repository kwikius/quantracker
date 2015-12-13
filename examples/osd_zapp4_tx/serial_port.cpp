
#include "resources.hpp"

extern "C" void USART1_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART1_IRQHandler()
{
   static_assert(
   std::is_same<
     mavlink_usart,quan::stm32::usart1
   >::value ,"invalid usart for serial_port irq");

   mavlink_tx_rx_task::irq_handler();
}


//






