/*
 Copyright (c) 2012 - 2013 Andy Little 

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

#include <type_traits>
#include "resources.hpp"

#error redo for boardtype4 USART3

#if (QUAN_OSD_BOARD_TYPE == 4)
extern "C" void USART3_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART3_IRQHandler()
#else
extern "C" void USART2_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART2_IRQHandler()
#endif
{
   static_assert(
   std::is_same<
#if (QUAN_OSD_BOARD_TYPE == 4)
     frsky_usart,quan::stm32::usart3
#else
      frsky_usart,quan::stm32::usart2
#endif
   >::value
   ,"invalid usart for serial_port irq");
   frsky_tx_rx_task::irq_handler();
}

#if (QUAN_OSD_BOARD_TYPE == 4)
extern "C" void USART1_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART1_IRQHandler()
#else
extern "C" void USART3_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART3_IRQHandler()
{
   static_assert(
   std::is_same<
#if (QUAN_OSD_BOARD_TYPE == 4)
     posdata_usart,quan::stm32::usart1
#else
     posdata_usart,quan::stm32::usart3
#endif
   >::value
   ,"invalid usart for serial_port irq");
   posdata_tx_rx_task::irq_handler();
}
