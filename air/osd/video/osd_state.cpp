
#include <stm32f4xx.h>
#include "osd_state.hpp"
#include "../resources.hpp"

namespace detail{
    bool dac_busy();
    void external_video_setup();
}


osd_state::state_t osd_state::m_current_state = osd_state::suspended;
bool osd_state::m_have_external_video = false;
void osd_state::suspend()
{ 
   
   __disable_irq();
    // disable dmas
   DMA1_Stream4->CR &= ~(1 << 0); // (EN)
   DMA1_Stream5->CR &= ~(1 << 0); // (EN)
   quan::stm32::module_reset<spi_clock_timer>();
   quan::stm32::module_reset<video_columns_gate_timer>();
   quan::stm32::module_reset<video_rows_line_counter>();
   quan::stm32::module_reset<sync_sep_timer>();
   // reset spis
   // spi2 and spi3 module reset
   quan::stm32::rcc::get()->apb1rstr |= (0b11 << 14);
   quan::stm32::rcc::get()->apb1rstr &= ~ (0b11 << 14);
   #if defined QUAN_OSD_TELEM_RECEIVER
   // reset av_telem_usart
   quan::stm32::module_reset<av_telem_usart>();
   #endif
   __enable_irq();
    // check dma's disabled
   while ( (DMA1_Stream4->CR & (1 << 0)) || (DMA1_Stream5->CR & (1 << 0))){;}
      // clear dma flags
   DMA1->HIFCR |= ( (0b111101 << 6) | (0b111101 << 0));
   DMA1->HIFCR &= ~ ( (0b111101 << 6) | (0b111101 << 0));

   while( detail::dac_busy() ){;}
   m_have_external_video = false;
   m_current_state = suspended;
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
    //call...
}
void osd_state::switch_to_external_video_mode()
{
   if ( m_current_state != suspended){
      suspend();
   }
   detail::external_video_setup();
   m_current_state = external_video;
}