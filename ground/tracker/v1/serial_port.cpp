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

#include <quan/stm32/usart/irq_handler.hpp>
#include "serial_ports.hpp"

extern "C" void USART3_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART3_IRQHandler()
{
   static_assert(
   std::is_same<
      sliprings::serial_port::usart_type,quan::stm32::usart3
   >::value
   ,"invalid usart for serial_port irq");

   quan::stm32::usart::irq_handler<sliprings::serial_port>();
}

extern "C" void UART4_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void UART4_IRQHandler()
{
   static_assert(
   std::is_same<
      av_fsk::serial_port::usart_type,quan::stm32::uart4
   >::value
   ,"invalid usart for serial_port irq");

   quan::stm32::usart::irq_handler<av_fsk::serial_port>();
}

#if 0
extern "C" void USART2_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART2_IRQHandler()
{
   static_assert(
   std::is_same<
      frsky::serial_port::usart_type,quan::stm32::usart2
   >::value
   ,"invalid usart for serial_port irq");

   quan::stm32::usart::irq_handler<frsky::serial_port>();
}
#endif








