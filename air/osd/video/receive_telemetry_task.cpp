/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korhonen(Openpilot.org)
   taulabs ( taulabs.com) 
   brainFPV ( brainfpv.com)
   Thomas Oldbury (super-osd.com)

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
#if !(defined QUAN_OSD_TELEM_RECEIVER)
#error only for use with telem receiver
#endif

#include <cstdio>
#include <cstring>

#include <stm32f4xx.h>

#include "FreeRTOS.h"
#include <task.h>

#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/min.hpp>

#include "resources.hpp"
#include "../../../air/osd/video/video_cfg.hpp"
#include "../../../air/osd/video/video_buffer.hpp"
#include "../rx_telemetry.hpp"

size_t get_telemetry_num_bytes() 
{ 
  return video_buffers::telem::rx::get_num_data_bytes();
}
size_t read_telemetry_data(char * buffer, size_t len)
{
    size_t len_out = quan::min(len,get_telemetry_num_bytes());
    memcpy(buffer,&video_buffers::telem::rx::manager.m_read_buffer->front(),len_out);
    return len_out;
}

  void on_telemetry_receive();
namespace detail{
  void create_telem_rx_swap_semaphores();
  void swap_telem_rx_buffers();
// should be the user level function
// equivalent to on_draw()
// now need a function to read the buffer

//  {
//      the_rx_telemetry.refresh();
//  }
}

namespace {

   void vsync_telem_rx_task(void* params)
   {
      for (;;){
         detail::swap_telem_rx_buffers();
         // this should be the user layer function
         on_telemetry_receive();
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;

}//namespace

void create_vsync_telem_rx_task()
{
   detail::create_telem_rx_swap_semaphores();
   the_rx_telemetry.init();
   xTaskCreate(
      vsync_telem_rx_task,"vsync_telem_rx_task", 
      600,
      &dummy_param,
      task_priority::vsync_telem_rx,
      &task_handle
   );
}

namespace{
   void vsync_telem_rx_task_usart_setup()
   {
      quan::stm32::module_enable<telem_cmp_enable_pin::port_type>();
      quan::stm32::apply<
         telem_cmp_enable_pin    // TIM2_CH2 or TIM2_CH4 for boardtype 4 (PA3)
         ,quan::stm32::gpio::mode::af1 // same for all boardtypes 
         ,quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::module_enable<av_telem_rx::port_type>();
      quan::stm32::apply<
         av_telem_rx
   #if (QUAN_OSD_BOARD_TYPE == 4)
         ,quan::stm32::gpio::mode::af8 // PC7  USART6_RX
   #else
         ,quan::stm32::gpio::mode::af7
   #endif
         ,quan::stm32::gpio::pupd::pull_up
      >();
      
      quan::stm32::module_reset<av_telem_usart>();
      quan::stm32::module_enable<av_telem_usart>();
      
      quan::stm32::apply<
         av_telem_usart
         ,quan::stm32::usart::asynchronous
         ,quan::stm32::usart::transmitter<false>
         ,quan::stm32::usart::receiver<true>
         ,quan::stm32::usart::baud_rate<2000000,false>
         ,quan::stm32::usart::parity::none
         ,quan::stm32::usart::data_bits<8>
         ,quan::stm32::usart::stop_bits<1>
         ,quan::stm32::usart::rts<false>
         ,quan::stm32::usart::cts<false>
         ,quan::stm32::usart::i_en::cts<false>
         ,quan::stm32::usart::i_en::lbd<false>
         ,quan::stm32::usart::i_en::txe<false>
         ,quan::stm32::usart::i_en::tc<false>
         ,quan::stm32::usart::i_en::rxne<false>
         ,quan::stm32::usart::i_en::idle<false>
         ,quan::stm32::usart::i_en::pe<false>
         ,quan::stm32::usart::i_en::error<false>
      >();
     // av_telem_usart::get()->cr3.setbit<6>(); //( DMAR)
      av_telem_usart::get()->cr1.setbit<2>(); // ( RE)
   }

   /*
 for boardtype 4 dma is on USART6  DMA2 Channel 5 stream 1 or 2
 ( use stream 1 to save stream 2 for
*/
   void vsync_telem_rx_task_dma_setup()
   {
      RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
      for ( uint8_t i = 0; i < 20; ++i){
         asm volatile ("nop" : : :);
      }
      RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA2RST;
      RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA2RST;
   #if (QUAN_OSD_BOARD_TYPE == 4)
      DMA_Stream_TypeDef *stream = DMA2_Stream1;
      constexpr uint32_t dma_channel = 5U;
   #else
      DMA_Stream_TypeDef *stream = DMA2_Stream5;
      constexpr uint32_t dma_channel = 4U;
   #endif
      if (  stream->CR & (1 << 0)){
         stream->CR &= ~(1 << 0); // (EN)
         while(stream->CR & (1 << 0)){;}
      }
      stream->CR = 
         ( dma_channel << 25)     // select channel
         | (0b10 << 16) // medium high priority
         |( 1 << 10) ;  // (MINC);      

      stream->FCR |= (1 << 2) ;// (DMDIS)
      // set threshold full
      stream->FCR |= (0b11 << 0);
      // setup periph_reg
      stream->PAR = (uint32_t)&av_telem_usart::get()->dr;
     // no irq so no NVIC
   }
} // namespace

void vsync_telem_rx_task_setup()
{
   vsync_telem_rx_task_usart_setup();
   vsync_telem_rx_task_dma_setup();
}
