#include <stm32f4xx.h>
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include "video_cfg.hpp"
#include "video_buffer.hpp"
#include "video.hpp"
#include <quan/conversion/itoa.hpp>

// columns gate_timer is TIM2
// start and end px ( from vsync rising edge
// in fact relates to video buf

// first visible pixel on osd line in pixel units from pixel 0
// pixel 0 hypothetically starts at rising edge of hsync when gated timer starts
// (start must give >= 8usec from hsync rising edge - TODO check this)
uint16_t video_cfg::columns::osd::m_begin = 90;
// one after last visible pixel in pixel units from pixel 0
uint16_t video_cfg::columns::osd::m_end = 778 ;

// first bit ( of start sequence) in bit units from bit 0
// bit 0 hypothetically starts at rising edge of hsync when gated timer starts
// say 2 MHz clock
// (start must give >= 8usec from hsync rising edge - TODO check this)
// must be divisable by 8
uint16_t video_cfg::columns::telem::m_begin = 12;
// One after last bit in bit units from bit 0
uint16_t video_cfg::columns::telem::m_end = 110;

namespace {
   #if QUAN_OSD_BOARD_TYPE==3
   static constexpr bool transmitter = true;
   #else
   static constexpr bool transmitter = false;
   #endif
   #if QUAN_OSD_BOARD_TYPE==1
   static constexpr bool receiver = true;
   #else
   static constexpr bool receiver = false;
   #endif
}
bool is_transmitter() { return transmitter;}
bool is_receiver(){return receiver;}
// called by row line_counter

// at first edge of hsync
//on start of next after last active row
void video_cfg::columns::disable()
{
   //change SMS to slave mode disabled
   gate_timer::get()->smcr &= ~ (0b111 << 0); /// (SMS)
   gate_timer::get()->sr.bb_clearbit<6>(); // (TIF)
   gate_timer::get()->dier.bb_clearbit<6>(); // (TIE)
}

// called on first edge of hsync
// at start of first osd row
void video_cfg::columns::osd::enable()
{
   // pixel clk timing
   spi_clock::timer::get()->cnt = 0;
   // div 2 for slower clk so compensate in faster bus clk
   auto const clks_px = video_cfg::spi_clock::get_timer_clks_per_px() /2 ;
   spi_clock::timer::get()->arr = clks_px*2-1; // faster bus clk
   spi_clock::timer::get()->ccr1 = clks_px-1; // faster bus clk
   // pixel clk gate timing
   gate_timer::get()->cnt = 0;
   gate_timer::get()->ccr2 = (m_begin * clks_px) - 1; // start px /2 as busclk is only half of pixel bus clk
   gate_timer::get()->arr = ( (m_end +7) * clks_px) - 1;  // end px /2 as busclk is only half of pixel bus clk
//
   // change gate to trigger mode ready for hsync second edge to start gate_timer
   gate_timer::get()->smcr |= (0b110 << 0); /// (SMS)

   gate_timer::get()->sr.bb_clearbit<6>(); //(TIF)
   gate_timer::get()->dier.bb_setbit<6>(); // (TIE)
   // flag set from main to swap buffers
   if (  ! video_buffers::osd::manager.swapped()) {
      video_buffers::osd::manager.swap();
   }
   video_buffers::osd::manager.read_reset();

   if (!video_cfg::rows::is_odd_frame()) {
      // odd is 1 even is 0
      // first is odd so inc if even
      video_buffers::osd::manager.read_advance (video_t::get_display_size_x_bytes() + 1);
   }
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
   spi_setup1<video_mux_out_white_spi>();
   
   video_mux_out_black_spi::get()->cr1.bb_clearbit<8>(); // SSI low for NSS low
   video_mux_out_white_spi::get()->cr1.bb_clearbit<8>(); // SSI low for NSS low
}
 
 
}//namespace
 
// called on first edge of hsync
// at start of first telem row
void video_cfg::columns::telem::enable()
{
   auto const clks_bit = spi_clock::get_telem_clks_per_bit() /2;
   if (is_transmitter()) {
      // get any data to buffer
      if (! video_buffers::telem::tx::manager.swapped()) {
         video_buffers::telem::tx::manager.swap();
         video_buffers::telem::tx::manager.read_reset();
        // video_buffers::telem::tx::m_want_tx= true;
         // pixel clk timing
         spi_clock::timer::get()->cnt = 0;
         // div 2 for slower clk so compensate in faster bus clk
         spi_clock::timer::get()->arr = clks_bit*2 - 1; // faster bus clk
         spi_clock::timer::get()->ccr1 = clks_bit -1; // faster bus clk
      }
   }
   // pixel timer gate timing
   gate_timer::get()->cnt = 0;
   gate_timer::get()->ccr2 = m_begin * clks_bit - 1;
   gate_timer::get()->arr = (m_end - 1)  * clks_bit - 1 ;

    // 
   if (is_receiver()) {
      gate_timer::get()->ccer.bb_setbit<4>();   ;//(CC2E)
   }
   gate_timer::get()->sr.bb_clearbit<6>();  // (TIF)
   gate_timer::get()->dier.bb_setbit<6>();  // (TIE)
   // change gate to trigger mode ready for TRGI edge to start gate_timer
   gate_timer::get()->smcr |= (0b110 << 0); /// (SMS)

   if ( is_receiver()){
       uint8_t * ptr = &video_buffers::telem::rx::manager.m_write_buffer->front();
       DMA2_Stream5->M0AR = (uint32_t)ptr;
       DMA2_Stream5->NDTR =  video_buffers::telem::rx::get_num_data_bytes();
       DMA2->HIFCR |= (0b111101 << 6) ;
       DMA2->HIFCR &= ~ (0b111101 << 6) ;
       
       av_telem_in_usart::get()->cr2.clearbit<14>(); //(LINEN)
       av_telem_in_usart::get()->cr3.setbit<6>(); //( DMAR)
       av_telem_in_usart::get()->cr3.setbit<11>(); //(ONEBIT)
       av_telem_in_usart::get()->sr = 0;
       av_telem_in_usart::get()->cr1.setbit<13>(); // ( UE)
       DMA2_Stream5->CR |= (1 << 0); // (EN)
   }
}
 void set_text_data( const char* text);
// called on second edge of hsync
// at next after last telem row
// poss could sample A2D value in centre of line here
// for black value
void video_cfg::columns::telem::disable()
{
   video_cfg::columns::disable();
  // video_buffers::telem::tx::m_want_tx= false;
   if (is_receiver()) {
      gate_timer::get()->ccer.bb_clearbit<4>();   ;//(CC2E)
/*
 got Framing error
 Overrun Error
 Noise Flag
*/
#if 0 
      char buf[30];
     
      if ((DMA2_Stream5->NDTR != video_buffers::telem::rx::get_num_data_bytes()) ){
      // || ((av_telem_in_usart::get()->sr.get() & (1 << 5)) !=0)){
         quan::itoasc(DMA2_Stream5->NDTR, buf,10);
        // set_text_data(buf);
      }else{
         //set_text_data("None");
      }
#endif
      DMA2_Stream5->CR &= ~(1 << 0); // (EN)
      av_telem_in_usart::get()->cr3.clearbit<6>(); //( DMAR)
      av_telem_in_usart::get()->cr1.clearbit<13>(); // ( UE)
     // av_telem_in_usart::get()->cr1.clearbit<2>(); // ( RXE)
    //  quan::stm32::disable<av_telem_in_usart>();
    //  quan::stm32::module_reset<av_telem_in_usart>();
      video_buffers::telem::rx::manager.swap();

   }
}
 
// called at second edge of hsync
// start of telem rows
void video_cfg::columns::telem::begin()
{
   // if tx configure dma for telem tx
   // if rx  configure dma for telem rx
  // if (video_buffers::telem::tx::m_want_tx) {
   if( is_transmitter()){
      uint8_t* const white = video_buffers::telem::tx::get_white_read_pos();
      uint16_t const dma_length = video_buffers::telem::tx::get_full_bytes_per_line()-1;
      DMA1_Stream5->M0AR = (uint32_t) (white+1);
      DMA1_Stream5->NDTR = dma_length  ;
      DMA1->HIFCR |= (0b111101 << 6) ;
      DMA1->HIFCR &= ~ (0b111101 << 6) ;
      
      // spi3 module enable reset
      quan::stm32::rcc::get()->apb1enr |= (0b1 << 15);
      quan::stm32::rcc::get()->apb1rstr |= (0b1 << 15);
      quan::stm32::rcc::get()->apb1rstr &= ~ (0b1 << 15);
      spi_setup1<video_mux_out_white_spi>();
      video_mux_out_white_spi::get()->cr1.bb_clearbit<8>(); // SSI low for NSS low
      video_mux_out_white_spi::get()->dr = white[0] | 0b00001111;
      video_mux_out_white_spi::get()->cr1.bb_setbit<6>(); //(SPE)
      DMA1_Stream5->CR |= (1 << 0); // (EN)
   }
   // receiver
   // enable usart dma
}
 
 
void video_cfg::columns::telem::end()
{
  // if (video_buffers::telem::tx::m_want_tx) {
   if( is_transmitter()){
      // gate_timer::get()->sr.bb_clearbit<6>();// TIF
      gate_timer::get()->dier.bb_setbit<6>(); // TIE
      video_buffers::telem::tx::manager.read_advance (video_buffers::telem::tx::get_full_bytes_per_line());
      gate_timer::get()->cnt = 0;
      video_cfg::spi_clock::timer::get()->cnt = 0;
      // reset spi3
      quan::stm32::rcc::get()->apb1enr &= ~ (0b1 << 15);
      quan::stm32::rcc::get()->apb1rstr |= (0b1 << 15);
      
      DMA1_Stream5->CR &= ~ (1 << 0); // (EN)
   }
}
// called at rising edge of hsync
// start of osd rows
void video_cfg::columns::osd::begin()
{
   uint8_t* const black = video_buffers::osd::get_black_read_pos() ;
   uint8_t* const white = video_buffers::osd::get_white_read_pos() ;
   
   uint16_t const dma_length = (video_t::get_display_size_x_bytes());
   black[dma_length+1] |= 0x0F;
   white[dma_length+1] |= 0x0F;
   
   DMA1_Stream4->M0AR = (uint32_t) (black+1);
   DMA1_Stream5->M0AR = (uint32_t) (white+1);
   
   DMA1_Stream4->NDTR = dma_length  ;
   DMA1_Stream5->NDTR = dma_length  ;
   
   DMA1->HIFCR |= ( (0b111101 << 6) | (0b111101 << 0));
   DMA1->HIFCR &= ~ ( (0b111101 << 6) | (0b111101 << 0));
   
   spi_ll_setup();
   
   video_mux_out_black_spi::get()->dr = black[0] | 1U;
   video_mux_out_white_spi::get()->dr = white[0] | 1U;
   video_mux_out_black_spi::get()->cr1.bb_setbit<6>(); //(SPE)
   video_mux_out_white_spi::get()->cr1.bb_setbit<6>(); //(SPE)
   DMA1_Stream4->CR |= (1 << 0); // (EN)
   DMA1_Stream5->CR |= (1 << 0); // (EN)
}
// at start of line, before visible pixels on line, setup dma
// also at start of telem data
void video_cfg::columns::tif_irq()
{
   gate_timer::get()->dier.bb_clearbit<6>();
   // assume transfer complete
   // so dma disabled
   if (video_cfg::rows::get_current_mode() == rows::mode::telemetry) {
      telem::begin();
   } else { //osd pixels output
      osd::begin();
   }
}
 
void video_cfg::columns::osd::end()
{
   gate_timer::get()->sr.bb_clearbit<6>();// TIF
   gate_timer::get()->dier.bb_setbit<6>(); // TIE
   video_buffers::osd::manager.read_advance ( (video_t::get_display_size_x_bytes() +1) *2);
   gate_timer::get()->cnt = 0;
   video_cfg::spi_clock::timer::get()->cnt = 0;
   // reset spi's
   quan::stm32::rcc::get()->apb1enr &= ~ (0b11 << 14);
   quan::stm32::rcc::get()->apb1rstr |= (0b11 << 14);
   // disable SPI DMA's
   DMA1_Stream4->CR &= ~ (1 << 0); // (EN)
   DMA1_Stream5->CR &= ~ (1 << 0); // (EN)
}
// at end of line
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
// compare on CC2 gives one shot PWM to gate pixel gate_timer
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
      cr2.ti1s = false; // TIM2_CH1 is connected to TI1
      cr2.mms = 0b101; // OC2REF is TRGO
      gate_timer::get()->cr2.set (cr2.value);
   }
    // smcr trigger polarity?
   {
      quan::stm32::tim::smcr_t smcr = gate_timer::get()->smcr.get();
      smcr.msm = false  ; // sync with external timers
      smcr.ts  = 0b101 ; // trigger source is filtered timer TI1FP1
      smcr.sms = 0b000 ; // no slave trigger till enabled ( by row line_counter)
      gate_timer::get()->smcr.set (smcr.value);
   }
   {
      quan::stm32::tim::ccmr1_t ccmr1 = gate_timer::get()->ccmr1.get();
      ccmr1.cc1s   = 0b01;   // IC1 is input mapped on TI1 (hsync)
      ccmr1.ic1psc = 0b00;   // no prescaler
      ccmr1.ic1f   = 0b0000; // no filter
      ccmr1.cc2s  = 0b00;   // OC2 is output mapped to TRGO ( enable px clk)
      ccmr1.oc2fe = false;
      ccmr1.oc2pe = false;
      ccmr1.oc2m  = 0b111;  // pwm mode 2
      gate_timer::get()->ccmr1.set (ccmr1.value);
   }
   {
      quan::stm32::tim::ccer_t ccer = gate_timer::get()->ccer.get();
// second edge of hsync TIM2_CH1 
#if (QUAN_OSD_BOARD_TYPE == 1) || (QUAN_OSD_BOARD_TYPE == 3)
      ccer.cc1np = false;  // Ti1FP1 rising edge trigger ( hsync)
      ccer.cc1p  = false;  // Ti1FP1 rising edge trigger
#else
   #if QUAN_OSD_BOARD_TYPE == 2
      ccer.cc1np = false;  // Ti1FP1 falling edge trigger ( hsync)
      ccer.cc1p  = true;  // Ti1FP1 falling edge trigger
   #else
      #error unknown board type
   #endif
#endif
      ccer.cc2p  = true;  // active low ???
      ccer.cc2e  = true;
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
   NVIC_EnableIRQ (TIM2_IRQn);
   // set priority very high
   // dont set CEN as it will be enabled by hsync on input trigger
}
 
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
 
 