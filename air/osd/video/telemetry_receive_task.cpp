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

#include <cstdio>
#include <cstring>

#include <stm32f4xx.h>

#include "FreeRTOS.h"
#include <task.h>

//#include <quan/dynarray.hpp>
#include <quan/time.hpp>
#include <quan/frequency.hpp>

#include "resources.hpp"
#include "../../../air/osd/video/video_cfg.hpp"
#include "../../../air/osd/video/video_buffer.hpp"

void create_telem_swap_semaphores();
void swap_telem_buffers();

namespace {

   uint8_t* telem_buffer = nullptr;
   bool init_telem_buffer()
   {
      telem_buffer = (uint8_t*) pvPortMalloc(video_buffers::telem::tx::get_num_data_bytes());
      return telem_buffer != nullptr;
   }

  

   void telem_rx_task(void* params)
   {
 // TODO
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;

}//namespace

void create_telem_rx_task()
{
   xTaskCreate(
      telem_task,"telem_task", 
      600,
      &dummy_param,
      task_priority::av_telemetry,
      &task_handle
   );
}

namespace {
 
   void av_telem_usart_setup()
   {
//todo redo transmitter using usart
#if defined QUAN_OSD_TELEM_RECEIVER
      quan::stm32::module_enable<telem_cmp_enable_pin::port_type>();
      quan::stm32::apply<
         telem_cmp_enable_pin    // TIM2_CH2 or TIM2_CH4 for boardtype 4 (PA3)
         ,quan::stm32::gpio::mode::af1 // same for all boardtypes 
         ,quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::module_enable<av_video_rxi::port_type>();
      quan::stm32::apply<
         av_video_rxi
#if (QUAN_OSD_BOARD_TYPE == 4)
         ,quan::stm32::gpio::mode::af8 // PC7  USART6_RX
#else
         ,quan::stm32::gpio::mode::af7
#endif
         ,quan::stm32::gpio::pupd::pull_up
      >();
      
      quan::stm32::module_reset<av_telemetry_usart>();
      quan::stm32::module_enable<av_telemetry_usart>();
      
      quan::stm32::apply<
         av_telemetry_usart
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
     // av_telemetry_usart::get()->cr3.setbit<6>(); //( DMAR)
      av_telemetry_usart::get()->cr1.setbit<2>(); // ( RE)
#else  // transmitter
  #if (QUAN_OSD_BOARD_TYPE == 4) 
      quan::stm32::module_enable<telem_cmp_enable_pin::port_type>();
      quan::stm32::apply<
         telem_cmp_enable_pin   
         ,quan::stm32::gpio::mode::input // Shutdown TLV3501 kep cmp disabled
         ,quan::stm32::gpio::pupd::pull_up
      >();
      quan::stm32::module_enable<av_telem_rx::port_type>();
      quan::stm32::apply<
         av_telem_rx   
         ,quan::stm32::gpio::mode::input //cmp output hiz pulled up
         ,quan::stm32::gpio::pupd::pull_up
      >();
  #endif
#endif
   }
}//namespace 

//void av_telem_dma_setup();
 
void av_telem_rx_setup()
{
   av_telem_usart_setup();
 //  av_telem_dma_setup();
}

 