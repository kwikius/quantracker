
#include <stm32f4xx.h>
#include "osd_state.hpp"
#include "../resources.hpp"

namespace detail{
#if !defined QUAN_AERFLITE_BOARD
    bool dac_busy();
#endif
    void video_setup();
    //void internal_video_mode_setup();
    void video_take_down();
    void reset_osd_swap_semaphores();
}

volatile osd_state::state_t osd_state::m_current_state = osd_state::suspended;
volatile bool osd_state::m_have_external_video = false;

void osd_state::suspend()
{ 
  // quan::stm32::set<heartbeat_led_pin>();
   taskENTER_CRITICAL();
   __disable_irq();
  // quan::stm32::module_disable<sync_sep_timer>();
   quan::stm32::module_reset<sync_sep_timer>();
  // quan::stm32::module_disable<spi_clock_timer>();
   quan::stm32::module_reset<spi_clock_timer>();
  // quan::stm32::module_disable<video_columns_gate_timer>();
   quan::stm32::module_reset<video_columns_gate_timer>();
   //quan::stm32::module_disable<video_rows_line_counter>();
   quan::stm32::module_reset<video_rows_line_counter>();

   // reset spi2 and spi3 
   quan::stm32::rcc::get()->apb1enr &= ~ (0b11 << 14);
   quan::stm32::rcc::get()->apb1rstr |= (0b11 << 14);
   quan::stm32::rcc::get()->apb1rstr &= ~(0b11 << 14);
   #if defined QUAN_OSD_TELEM_RECEIVER
   //quan::stm32::module_disable<av_telem_usart>();
   quan::stm32::module_reset<av_telem_usart>();
   #endif
   detail::video_take_down();
   __enable_irq();

   detail::reset_osd_swap_semaphores();
   
    // check dma's disabled
   while (DMA1_Stream4->CR & (1 << 0)){
      DMA1_Stream4->CR &= ~(1<<0); // (EN)
   }
   while (DMA1_Stream5->CR & (1 << 0)){   
      DMA1_Stream5->CR &= ~(1 << 0); // (EN) 
   }
   #if defined QUAN_OSD_TELEM_RECEIVER
   while ( DMA2_Stream1->CR & (1 << 0)){
      DMA2_Stream1->CR &= ~(1 << 0); 
   }
   #endif
      // clear dma flags
   DMA1->HIFCR |= ( (0b111101 << 6) | (0b111101 << 0));
   DMA1->HIFCR &= ~( (0b111101 << 6) | (0b111101 << 0));
#if defined QUAN_OSD_TELEM_RECEIVER
   DMA2->LIFCR |= (0b111101 << 6) ; // clear flags for Dma2 Stream 1
   DMA2->LIFCR &= ~(0b111101 << 6) ; // flags for Dma2 Stream 1
#endif

   m_have_external_video = false;
   m_current_state = suspended;
   taskEXIT_CRITICAL();
#if !defined QUAN_AERFLITE_BOARD
   while( detail::dac_busy() ){;}
#endif

}

void osd_state::set(state_t state)
{
   if ( state != m_current_state){
      switch( state ){
         case external_video:
            switch_to_external_video_mode();
         break;
         case internal_video:
            switch_to_internal_video_mode();
         break;
         case suspended:
           osd_state::suspend();
         break;
         default:
         break;
      }
   }
}

void osd_state::switch_to_internal_video_mode()
{
    if ( m_current_state != suspended){
      suspend();
    }
    m_current_state = internal_video;
    detail::video_setup();

}

void osd_state::switch_to_external_video_mode()
{
   if ( m_current_state != suspended){
      suspend();
   }
   m_current_state = external_video;
   detail::video_setup();
}