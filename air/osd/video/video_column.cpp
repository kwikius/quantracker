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
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "resources.hpp"
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include "video_cfg.hpp"
#include "video_buffer.hpp"
#include "osd_state.hpp"
#include <quan/conversion/itoa.hpp>

namespace {

   SemaphoreHandle_t h_request_osd_buffers_swap = 0U;
   SemaphoreHandle_t h_osd_buffers_swapped = 0U;
   BaseType_t HigherPriorityTaskWoken_osd = 0U;

#if ((defined QUAN_OSD_TELEM_TRANSMITTER) || (defined QUAN_OSD_TELEM_RECEIVER))
// used for tx and rx telem
   SemaphoreHandle_t h_request_telem_buffers_swap = 0U;
   SemaphoreHandle_t h_telem_buffers_swapped = 0U;
   BaseType_t HigherPriorityTaskWoken_telem = 0U;
#endif
}

namespace detail{

   void create_osd_swap_semaphores()
   {
      h_request_osd_buffers_swap = xSemaphoreCreateBinary();
      h_osd_buffers_swapped = xSemaphoreCreateBinary();
   }

   void reset_osd_swap_semaphores()
   {
      if (h_request_osd_buffers_swap != 0){
         while (xSemaphoreGive(h_request_osd_buffers_swap) == pdTRUE){;}
      }
      if (h_osd_buffers_swapped != 0){
         while (xSemaphoreGive(h_osd_buffers_swapped)== pdTRUE){;}
      }
   }

   bool swap_osd_buffers(quan::time::ms const & wait_time)
   {
      xSemaphoreGive(h_request_osd_buffers_swap);
      TickType_t const wait_time1 = static_cast<TickType_t>(wait_time.numeric_value());
      if ( xSemaphoreTake(h_osd_buffers_swapped,wait_time1) == pdTRUE){
         video_buffers::osd::clear_write_buffer();
         return true;
      }else{
         return false;
      }
   }

      //called from draw_task
   void swap_osd_buffers()
   {
      xSemaphoreGive(h_request_osd_buffers_swap);
      xSemaphoreTake(h_osd_buffers_swapped,portMAX_DELAY);
      video_buffers::osd::clear_write_buffer();
   }

#if defined QUAN_OSD_TELEM_TRANSMITTER
   void create_telem_tx_swap_semaphores()
   {
      h_request_telem_buffers_swap = xSemaphoreCreateBinary();
      h_telem_buffers_swapped = xSemaphoreCreateBinary();
   }

   // call from task
   void swap_telem_tx_buffers()
   {
      xSemaphoreGive(h_request_telem_buffers_swap);
      xSemaphoreTake(h_telem_buffers_swapped,portMAX_DELAY);
      video_buffers::telem::tx::reset_write_buffer();
   }
#endif

#if defined QUAN_OSD_TELEM_RECEIVER
   void create_telem_rx_swap_semaphores()
   {
      h_request_telem_buffers_swap = xSemaphoreCreateBinary();
      h_telem_buffers_swapped = xSemaphoreCreateBinary();
   }

   // call (indirectly ) from task level
   // arguably could make this the 
   // task function
   void swap_telem_rx_buffers()
   {
      // clears the read buffer to 0
      video_buffers::telem::rx::reset_read_buffer();
      // Tell IRQ layer that its ok to swap buffers
      xSemaphoreGive(h_request_telem_buffers_swap);
      // block till buffers swapped
      xSemaphoreTake(h_telem_buffers_swapped,portMAX_DELAY);
   }
#endif
} // detail

namespace {
// call from ISR

   void service_osd_buffers()
   {
     HigherPriorityTaskWoken_osd = pdFALSE;
     if(xSemaphoreTakeFromISR(h_request_osd_buffers_swap,NULL) == pdTRUE){    
        video_buffers::osd::manager.swap();
        xSemaphoreGiveFromISR(h_osd_buffers_swapped,&HigherPriorityTaskWoken_osd);
     }
   }
#if defined QUAN_OSD_TELEM_TRANSMITTER
   void service_telem_tx_buffers()
   {  
      HigherPriorityTaskWoken_telem = pdFALSE;
      if(xSemaphoreTakeFromISR(h_request_telem_buffers_swap,NULL) == pdTRUE){
        video_buffers::telem::tx::manager.swap();
        xSemaphoreGiveFromISR(h_telem_buffers_swapped,&HigherPriorityTaskWoken_telem);
     }
   }
#endif

#if defined QUAN_OSD_TELEM_RECEIVER
   // if the task layer has read the previous buffer
   // then it 'gives' the h_request_osd_buffers_swap semaphore
   // else (If the new data arrives before the task has had time to read)
   // no buffer swap takes place
   // so the data is ignored. The same buffer will overwritten again
   void service_telem_rx_buffers()
   {  
      HigherPriorityTaskWoken_telem = pdFALSE;
      if(xSemaphoreTakeFromISR(h_request_telem_buffers_swap,NULL) == pdTRUE){
          video_buffers::telem::rx::manager.swap();
          xSemaphoreGiveFromISR(h_telem_buffers_swapped,&HigherPriorityTaskWoken_telem);
      }
   }
#endif
}

// columns gate_timer is TIM2
// start and end px ( from vsync rising edge
// in fact relates to video buf

// first visible pixel on osd line in pixel units from pixel 0
// pixel 0 hypothetically starts at 2nd edge of hsync when gated timer starts
// (start of line must be greater than time_from_second_edge = 5.75 usec from second edge of hsync
// mbegin  >= time_from_second_edge * pixel_clk_frequency;

#if defined (QUAN_DISPLAY_INTERLACED)
uint16_t video_cfg::columns::osd::m_begin = 90;
// one after last visible pixel in pixel units from pixel 0
// time_from_second_edge_to_eol = 64us - time_from_second_edge
// medn <= time_from_second_edge_to_eol * pixel_clk_frequency
uint16_t video_cfg::columns::osd::m_end = 778 ;
#else
// not interlaced
uint16_t video_cfg::columns::osd::m_begin = 45;
uint16_t video_cfg::columns::osd::m_end = 389;
#endif

// first bit ( of start sequence) in bit units from bit 0
// bit 0 hypothetically starts at rising edge of hsync when gated timer starts
// say 2 MHz clock
// (start must give >= 8usec from hsync rising edge - TODO check this)
// must be divisable by 8
uint16_t video_cfg::columns::telem::m_begin = 12;
// One after last bit in bit units from bit 0
uint16_t video_cfg::columns::telem::m_end = 110;

// called on first edge of hsync
// on start of next after last active row
// by line_counter.compare in external mode
// and in timer3.uif in internal mode
void video_cfg::columns::disable()
{
   //change SMS to slave mode disabled
   gate_timer::get()->smcr &= ~ (0b111 << 0); /// (SMS)
   gate_timer::get()->sr.bb_clearbit<6>(); // (TIF)
   gate_timer::get()->dier.bb_clearbit<6>(); // (TIE)
}

// called on first edge of hsync
// at start of first osd row
// by line_counter.compare in external mode
// and in timer3.uif in internal mode
//########################check use of TIM2 Ch2 and ch4 here //#################
void video_cfg::columns::osd::enable()
{
   // pixel clk timing
   spi_clock::timer::get()->cnt = 0;
   // div 2 for slower clk so compensate in faster bus clk
   auto const clks_px = spi_clock::get_timer_clks_per_px() /2 ;
   spi_clock::timer::get()->arr = clks_px*2-1; // faster bus clk
   spi_clock::timer::get()->ccr1 = clks_px-1; // faster bus clk
   // pixel clk gate timing
   gate_timer::get()->cnt = 0;
   gate_timer::get()->ccr4 = (m_begin * clks_px) - 1; // start px /2 as busclk is only half of pixel bus clk
   gate_timer::get()->arr = ( (m_end +7) * clks_px) - 1;  // end px /2 as busclk is only half of pixel bus clk
//###################### internal or external_mode ##########
   // change gate to trigger mode ready for hsync second edge to start gate_timer
   gate_timer::get()->smcr |= (0b110 << 0); /// (SMS)
//###########################################################
   gate_timer::get()->sr.bb_clearbit<6>(); //(TIF)
   gate_timer::get()->dier.bb_setbit<6>(); // (TIE)

   service_osd_buffers();

   video_buffers::osd::manager.read_reset();

#if defined (QUAN_DISPLAY_INTERLACED)
   if (!rows::is_odd_frame()) {
      // odd is 1 even is 0
      // first is odd so inc if even
      video_buffers::osd::manager.read_advance (get_display_size_x_bytes() + 1);
   }
#endif
   portEND_SWITCHING_ISR(HigherPriorityTaskWoken_osd);
}

// called on first edge of hsync
// at one after last osd row
void video_cfg::columns::osd::disable()
{
   video_cfg::columns::disable();
}
 
namespace {
   template <typename spi>
   void spi_setup1()
   {
   #if 1
      uint32_t constexpr or_mask = (1 << 15) | (1 << 14) | (1 << 9) | (1 << 8) | (1 << 7);
      
      spi::get()->cr1.set (or_mask);
      spi::get()->cr2.set (1 << 1); // (TXDMAEN)
   #else
      auto cr1 = spi::get()->cr1.get();
      // one directional mode
      cr1 |= (1 << 15) ; // (BIDIMODE);
      // tx only output enabled
      // maybe should be input here?
      cr1 |= (1 << 14) ; // (BIDIOE)
      
      // internal slave mode dont use NSS pin
      cr1 |= (1 << 9) ; // (SSM)
      /////////////////////////////////
      // slave is selected when NSS is low
      // clear internal NSS pin for slave mode enabled?
      // set high here so MISO is input
      cr1 |= (1 << 8) ; // (SSI)
      //////////////////////////////////////
      // LSB txed 1st
      cr1 |= (1 << 7) ; // (LSBFIRST)
      // disable spi for now
      cr1 &= ~ (1 << 6); // (SPE)
      cr1 &= ~ (0b111 << 3);
      //cr1 |= (baudrate << 3); // changes time for MISO to go hiZ
      // select slave mode
      cr1 &= ~ (1 << 2) ; // (MSTR)
      // clock low idle
      cr1 &= ~ (1<< 1); // (CPOL)
      // data valid on first rising edge
      cr1 &= ~ (1 << 0) ; // (CPHA)
      // no crc
      cr1 &= ~ (1 << 13); // (CRCEN)
      // no next crc
      cr1 &= ~ (1 << 12); // (CRCNEXT)
      // 8 bit data
      cr1 &= ~ (1 << 11); //(DFF)
      spi::get()->cr1 = cr1;
      spi::get()->cr2 |= (1 << 1); // (TXDMAEN)
   #endif
      
   }
    
   void spi_ll_setup()
   {
      // spi2 and spi3 module enable
      quan::stm32::rcc::get()->apb1enr |= (0b11 << 14);
      quan::stm32::rcc::get()->apb1rstr |= (0b11 << 14);
      quan::stm32::rcc::get()->apb1rstr &= ~ (0b11 << 14);
      
      spi_setup1<video_mux_out_black_spi>();
      video_mux_out_black_spi::get()->cr1.bb_clearbit<8>(); // SSI low for NSS low

      if( osd_state::get() == osd_state::external_video){
         spi_setup1<video_mux_out_white_spi>();
         video_mux_out_white_spi::get()->cr1.bb_clearbit<8>(); // SSI low for NSS low
      }
   }
 
}//namespace
 
// called on first edge of hsync
// at start of first telem row
void video_cfg::columns::telem::enable()
{
   auto const clks_bit = spi_clock::get_telem_clks_per_bit() /2;

#if defined QUAN_OSD_TELEM_TRANSMITTER
#pragma message "need to redo this for internal video modes"
   service_telem_tx_buffers();
   video_buffers::telem::tx::manager.read_reset();
   // pixel clk timing
   spi_clock::timer::get()->cnt = 0;
   // div 2 for slower clk so compensate in faster bus clk
   spi_clock::timer::get()->arr = clks_bit*2 - 1; // faster bus clk
   spi_clock::timer::get()->ccr1 = clks_bit -1; // faster bus clk
#endif
   // pixel timer gate timing
   gate_timer::get()->cnt = 0;
   gate_timer::get()->ccr4 = m_begin * clks_bit - 1;
   gate_timer::get()->arr = (m_end - 1)  * clks_bit - 1 ;
#if defined QUAN_OSD_TELEM_RECEIVER
   gate_timer::get()->ccer.bb_setbit<12>();//( CC4E) // gate timer output enable cmp TIM2_CH4
#endif
   gate_timer::get()->sr.bb_clearbit<6>();  // (TIF)
   gate_timer::get()->dier.bb_setbit<6>();  // (TIE)
   // change gate to trigger mode ready for TRGI edge to start gate_timer
   gate_timer::get()->smcr |= (0b110 << 0); /// (SMS)
#if defined QUAN_OSD_TELEM_RECEIVER
   uint8_t * ptr = &video_buffers::telem::rx::manager.m_write_buffer->front();
   DMA_Stream_TypeDef * stream = DMA2_Stream1; // USART6_RX DMA2 Stream 1 Cha 5
   stream->M0AR = (uint32_t)ptr;
   stream->NDTR =  video_buffers::telem::rx::get_num_data_bytes();
   DMA2->LIFCR |= (0b111101 << 6) ; // clear flags for Dma2 Stream 1
   DMA2->LIFCR &= ~ (0b111101 << 6) ; // flags for Dma2 Stream 1     
   av_telem_usart::get()->cr2.clearbit<14>(); //(LINEN)
   av_telem_usart::get()->cr3.setbit<6>(); //( DMAR)
   av_telem_usart::get()->cr3.setbit<11>(); //(ONEBIT)
   av_telem_usart::get()->sr = 0;
   av_telem_usart::get()->cr1.setbit<13>(); // ( UE)
   stream->CR |= (1 << 0); // (EN)
#endif // defined QUAN_OSD_TELEM_RECEIVER
#if defined QUAN_OSD_TELEM_TRANSMITTER
   portEND_SWITCHING_ISR(HigherPriorityTaskWoken_telem);
#endif
}
 //void set_text_data( const char* text);
// called on second edge of hsync
// at next after last telem row
// poss could sample A2D value in centre of line here
// for black value
void video_cfg::columns::telem::disable()
{
   video_cfg::columns::disable();

#if defined QUAN_OSD_TELEM_RECEIVER
// channel 4 on board_type 4
   gate_timer::get()->ccer.bb_clearbit<12>();//( CC4E)
    //###########DEBUG############################
//      if (DMA2_Stream1->NDTR == video_buffers::telem::rx::get_num_data_bytes()){
//         quan::stm32::clear<heartbeat_led_pin>();
//      }else{
//         quan::stm32::set<heartbeat_led_pin>();
//      }
      // disable the usart DMA
      DMA2_Stream1->CR &= ~(1 << 0); // (EN)      av_telem_usart::get()->cr3.clearbit<6>(); //( DMAR)
      av_telem_usart::get()->cr1.clearbit<13>(); // ( UE)
      service_telem_rx_buffers();
      portEND_SWITCHING_ISR(HigherPriorityTaskWoken_telem);

 #endif
}
 
// called at second edge of hsync
// at start of each telem row
void video_cfg::columns::telem::begin()
{
#if defined QUAN_OSD_TELEM_TRANSMITTER

      uint8_t* const white = video_buffers::telem::tx::get_white_read_pos();
      uint16_t const dma_length = video_buffers::telem::tx::get_full_bytes_per_line()-1;
// same for all boards
      if (osd_state::get() == osd_state::external_video){
         // in internal mode need to send out of black stream
         DMA1_Stream5->M0AR = (uint32_t) (white+1);
         DMA1_Stream5->NDTR = dma_length  ;
         DMA1->HIFCR |= (0b111101 << 6) ;
         DMA1->HIFCR &= ~ (0b111101 << 6) ;
         // spi3 module enable and reset
         quan::stm32::rcc::get()->apb1enr |= (0b1 << 15);
         quan::stm32::rcc::get()->apb1rstr |= (0b1 << 15);
         quan::stm32::rcc::get()->apb1rstr &= ~ (0b1 << 15);
         spi_setup1<video_mux_out_white_spi>();
         video_mux_out_white_spi::get()->cr1.bb_clearbit<8>(); // SSI low for NSS low
         video_mux_out_white_spi::get()->dr = white[0] | 0b00001111;
         video_mux_out_white_spi::get()->cr1.bb_setbit<6>(); //(SPE)
         DMA1_Stream5->CR |= (1 << 0); // (EN)
      }else{
          // only black channel TODO use usart for tx
          // use spi2 module for telemetry transmission
          DMA1_Stream4->M0AR = (uint32_t) (white+1);
          DMA1_Stream4->NDTR = dma_length  ;
          DMA1->HIFCR |= (0b111101 << 0) ;
          DMA1->HIFCR &= ~ (0b111101 << 0) ;
/////////////////////////////////////////////////////////
          quan::stm32::rcc::get()->apb1enr |= (0b1 << 14);
        //try change to   quan::stm32::rcc::get()->apb1enr.bb_setbit<14>();
         // ...etc
//###############################################
          quan::stm32::rcc::get()->apb1rstr |= (0b1 << 14);
          quan::stm32::rcc::get()->apb1rstr &= ~ (0b1 << 14);
          spi_setup1<video_mux_out_black_spi>();
          video_mux_out_black_spi::get()->cr1.bb_clearbit<8>(); // SSI low for NSS low
          video_mux_out_black_spi::get()->dr = white[0] | 0b00001111;
          video_mux_out_black_spi::get()->cr1.bb_setbit<6>(); //(SPE)
          DMA1_Stream4->CR |= (1 << 0); // (EN)
 
      }
#endif

}

// called at end of each telem row (gate_timer one shot uif)
void video_cfg::columns::telem::end()
{
#if defined QUAN_OSD_TELEM_TRANSMITTER

      gate_timer::get()->dier.bb_setbit<6>(); // TIE
      video_buffers::telem::tx::manager.read_advance (video_buffers::telem::tx::get_full_bytes_per_line());
      gate_timer::get()->cnt = 0;
      video_cfg::spi_clock::timer::get()->cnt = 0;
      if (osd_state::get() == osd_state::external_video){
         // reset spi3
         quan::stm32::rcc::get()->apb1enr &= ~ (0b1 << 15);
         quan::stm32::rcc::get()->apb1rstr |= (0b1 << 15);
         DMA1_Stream5->CR &= ~ (1 << 0); // (EN)
      }else{
#pragma message "need to redo this for internal video modes"
      }
 #endif
}
// called at rising edge of hsync
// start of each osd row gat timer TIF .
// either by external trigger or by internal rising edge
void video_cfg::columns::osd::begin()
{
   uint16_t const dma_length = (video_cfg::get_display_size_x_bytes());

   uint8_t* const black = video_buffers::osd::get_black_read_pos() ;
   black[dma_length+1] |= 0x0F;
   DMA1_Stream4->M0AR = (uint32_t) (black+1);
   DMA1_Stream4->NDTR = dma_length  ;

   if( osd_state::get() == osd_state::external_video){
      uint8_t* const white = video_buffers::osd::get_white_read_pos() ;
      white[dma_length+1] |= 0x0F;
      video_mux_out_white_spi::get()->dr = white[0] | 1U;
      video_mux_out_white_spi::get()->cr1.bb_setbit<6>(); //(SPE)
      DMA1_Stream5->M0AR = (uint32_t) (white+1);
      DMA1_Stream5->NDTR = dma_length  ;
   }
   DMA1->HIFCR |= ( (0b111101 << 6) | (0b111101 << 0));
   DMA1->HIFCR &= ~ ( (0b111101 << 6) | (0b111101 << 0));
   spi_ll_setup();
   
   video_mux_out_black_spi::get()->dr = black[0] | 1U;
   video_mux_out_black_spi::get()->cr1.bb_setbit<6>(); //(SPE)
   if( osd_state::get() == osd_state::external_video){
      DMA1_Stream4->CR |= (1 << 0); // (EN)
      DMA1_Stream5->CR |= (1 << 0); // (EN)
   }else{
      DMA1_Stream4->CR |= (1 << 0); // (EN)
   }
 
}
// at start of line, before visible pixels on line, setup dma
// also at start of telem data
void video_cfg::columns::tif_irq()
{
   gate_timer::get()->dier.bb_clearbit<6>(); // (TIE)

   if (video_cfg::rows::get_current_mode() == rows::mode::telemetry) {
      telem::begin();
   } else { //osd pixels output
      osd::begin();
   }
}
 
// called at end of each telem or osd line
// by gate_timer.uif
void video_cfg::columns::osd::end()
{
   gate_timer::get()->sr.bb_clearbit<6>();// TIF
   gate_timer::get()->dier.bb_setbit<6>(); // TIE
#if defined (QUAN_DISPLAY_INTERLACED)
   video_buffers::osd::manager.read_advance ( (video_cfg::get_display_size_x_bytes() +1) *2);
#else
   video_buffers::osd::manager.read_advance ( (video_cfg::get_display_size_x_bytes() +1));
#endif
   gate_timer::get()->cnt = 0;
   video_cfg::spi_clock::timer::get()->cnt = 0;
   // reset spi's
   quan::stm32::rcc::get()->apb1enr &= ~ (0b11 << 14);
   quan::stm32::rcc::get()->apb1rstr |= (0b11 << 14);
   // disable SPI DMA's
   if( osd_state::get() == osd_state::external_video){
      DMA1_Stream4->CR &= ~ (1 << 0); // (EN)
      DMA1_Stream5->CR &= ~ (1 << 0); // (EN)
   }else{
      DMA1_Stream4->CR &= ~ (1 << 0); // (EN)
   }
}
// called at end of each telem or osd line
// by gate_timer.uif
void video_cfg::columns::uif_irq()
{
   if (video_cfg::rows::get_current_mode() == rows::mode::telemetry) {
      telem::end();
   } else {
      osd::end();
   }
}
// columns gate gate_timer on TIM2
// triggered by hsync (TIM2_CH1) tim2_hsync_pin when enabled
// compare on CC2 (CC4 for boardtype 4) gives one shot PWM to gate pixel gate_timer

void video_cfg::columns::takedown()
{
     NVIC_DisableIRQ (TIM2_IRQn);
}

void video_cfg::columns::setup()
{
   quan::stm32::module_enable<gate_timer>();
   quan::stm32::module_reset<gate_timer>();
   {
      quan::stm32::tim::cr1_t cr1 = 0;
      cr1.cen  = false; //false as triggered by ITR
      cr1.udis = false;
      cr1.urs  = true; // only overflow generates update
      cr1.opm  = true; // stop after update
      cr1.dir  = false; // upcounting
      cr1.cms  = 0b00;  //edge aligned
      cr1.arpe = false; //
      cr1.ckd  = 0b00;
      gate_timer::get()->cr1.set (cr1.value);
   }
   {
      quan::stm32::tim::cr2_t cr2 = gate_timer::get()->cr2.get();
      cr2.ti1s = false; // TIM2_CH1 is connected to TI1 only not xored
      cr2.mms = 0b111;// OC4REF is TRGO
      gate_timer::get()->cr2.set (cr2.value);
   }
   {
      quan::stm32::tim::smcr_t smcr = gate_timer::get()->smcr.get();
      smcr.msm = false  ; // no sync with external timers
      if( osd_state::get() == osd_state::internal_video){
//########################## INTERNAL VIDEO ######################################
      // src in TIM3 is the second edge of the sync pulse
      // rising edge created by taking dac addr from 00 to 01
      // created by pwm in TIM3 
        smcr.ts  = 0b010 ; // internal video trigger source is TIM3 TRGO mapped to ITR2 
      }else {
//##########################EXTERNAL VIDEO########################################
         smcr.ts  = 0b101 ; // external video trigger source is filtered timer TI1FP1
      }
//#########################################################
      // sms is set to trigger mode (0b110) at start of telem/ osd rows
      smcr.sms = 0b000 ; // no slave trigger till enabled ( by row line_counter or TIM3 TRGO)
      gate_timer::get()->smcr.set (smcr.value);
   }
   {
      quan::stm32::tim::ccmr1_t ccmr1 = gate_timer::get()->ccmr1.get();

      if  ( osd_state::get() == osd_state::external_video){
         ccmr1.cc1s   = 0b01;   // IC1 is input mapped on TI1 (hsync)
      }else{
          // dont think we need to config ccmr1 cc1
         // this isnt connected to a pin in internal video mode
         ccmr1.cc1s   = 0b00; // output 
      }
      ccmr1.ic1psc = 0b00;   // no prescaler
      ccmr1.ic1f   = 0b0000; // no filter
      gate_timer::get()->ccmr1.set (ccmr1.value);
   }

   {
      quan::stm32::tim::ccmr2_t ccmr2 = gate_timer::get()->ccmr2.get();
      ccmr2.cc4s  = 0b00;   // OC4 is output mapped to TRGO ( enable px clk)
      ccmr2.oc4fe = false;
      ccmr2.oc4pe = false;
      ccmr2.oc4m  = 0b111;  // pwm mode 2
      gate_timer::get()->ccmr2.set (ccmr2.value);
 
   }
   {
      quan::stm32::tim::ccer_t ccer = gate_timer::get()->ccer.get();
//##################### INTERNAL VIDEO ##############################
// cc1 not needed
//##################### EXTERNAL VIDEO ##############################
// second edge of hsync TIM2_CH1 
      // dont care if internal video mode
      ccer.cc1np = false;  // Ti1FP1 rising edge trigger ( hsync)
      ccer.cc1p  = false;  // Ti1FP1 rising edge trigger
      ccer.cc4p  = true;  // active low ???
      ccer.cc4e  = true;
      gate_timer::get()->ccer.set (ccer.value);
   }
   gate_timer::get()->psc = 0;
   {
      quan::stm32::tim::dier_t dier = gate_timer::get()->dier.get();
      dier.tie = true;
      dier.uie = true;
      gate_timer::get()->dier.set (dier.value);
   }
   gate_timer::get()->sr.set (0);
   NVIC_SetPriority(TIM2_IRQn,interrupt_priority::video);
   NVIC_EnableIRQ (TIM2_IRQn);
   // dont set CEN as it will be enabled by hsync on input trigger
}
 
// tim2 is gate timer
// used to gate the pixel clock
extern "C" void TIM2_IRQHandler() __attribute__ ( (interrupt ("IRQ")));
 
extern "C" void TIM2_IRQHandler()
{
   typedef video_cfg::columns columns;
   if (columns::gate_timer::get()->sr.bb_getbit<6>()) {  // (TIF)
      columns::gate_timer::get()->sr.bb_clearbit<6>(); // (TIF)
      columns::tif_irq();
      return;
   }
   //add cc1if for usart
   if (columns::gate_timer::get()->sr.bb_getbit<0>()) {  // (UIF)
      columns::gate_timer::get()->sr.bb_clearbit<0>(); // (UIF)
      columns::uif_irq();
      return;
   }
}
