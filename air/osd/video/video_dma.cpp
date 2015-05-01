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

#include <stm32f4xx.h>
#include "../resources.hpp"

namespace {

   // pixel dma

   template <typename spi>
   void ll_pixel_dma_setup(DMA_Stream_TypeDef *stream, uint8_t channel)
   {
       if (  stream->CR & (1 << 0)){
         stream->CR &= ~(1 << 0); // (EN)
         while(stream->CR & (1 << 0)){;}
       }
         
         stream->CR =
            //select channel 
          //  cr &= ~(0b111 << 25); // (CHSEL)
            (channel << 25)
            // memory Burst mode 32 bits
          //  cr &= ~(0b11 << 23) ;// (MBURST);
           // cr |= (0b01 << 23) ;// (MBURST);
            // peripheral burst mode single
           // cr &= ~(0b11 << 21); // (PBURST);
            // dont change buffers
           // cr &= ~(1 << 19);// (CT)
            // no double buffer
           // cr &= ~(1 << 18);// (DBM)
            // highest priority
            | (0b11 << 16)  // (PL)
            // memory size 8 bits
           // cr &= ~(0b11 << 13); // (MSIZE)
            // peripheral size 8 bits
           // cr &= ~(0b11 << 11); // (PSIZE)
            // inc mem after each transfer
            | ( 1 << 10)  // (MINC); 
            // but not peripheral
           // cr &= ~(1 << 9) ;// (PINC)
            // no circular mode
           // cr &= ~(1 << 8); // (CIRC)
            // direction mem to peripheral
           // cr &= ~(0b11 << 6); //(DIR)
            | (0b01 << 6)
            // DMA controls flow
           // cr &= ~(1 << 5); //(PFCTRL)
         ;
         // clear direct mode
         stream->FCR |= (1 << 2) ;// (DMDIS)
         // set threshold full
         stream->FCR |= (0b11 << 0);
         // setup periph_reg
         stream->PAR = (uint32_t)&spi::get()->dr;
        // enable NVIC on channel?
   }
}//namespace 

void pixel_dma_setup()
{
   RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
   for ( uint8_t i = 0; i < 20; ++i){
      asm volatile ("nop" : : :);
   }
   RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
   RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA1RST;
  
   ll_pixel_dma_setup<video_mux_out_black_spi>(DMA1_Stream4,0); // spix tx
   ll_pixel_dma_setup<video_mux_out_white_spi>(DMA1_Stream5,0); // spix tx

// not needed? 
  // NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  // NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}
/*
 for boardtype 4 dma is on USART6  DMA2 Channel 5 stream 1 or 2
 ( use stream 1 to save stream 2 for
*/

#if (defined QUAN_OSD_TELEM_RECEIVER)
void av_telem_rx_dma_setup()
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
#endif



