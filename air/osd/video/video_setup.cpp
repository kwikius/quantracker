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
#include <misc.h>
#include "video_cfg.hpp"
#include "video_buffer.hpp"
#include "../resources.hpp"
#include "osd_state.hpp"

namespace {
   void hsync_setup()
   {
        // hsync input pins connected externally
        // TIx input for resetting hsync capture timer
        // Tim2 is gate timer ch1 second edge starts gate timer
        quan::stm32::module_enable<video_in_tim2_hsync_pin::port_type>();
        // TIM2_CH1 same on all boards
        quan::stm32::apply<
           video_in_tim2_hsync_pin,
           quan::stm32::gpio::mode::af1
        >();
        // gate trigger same on all boards
        quan::stm32::module_enable<video_in_tim3_hsync_pin::port_type>();
        // TIM3_ETR
        quan::stm32::apply<
           video_in_tim3_hsync_pin,
           quan::stm32::gpio::mode::af2
        >();
   }

   void hsync_take_down()
   {
        quan::stm32::apply<
           video_in_tim2_hsync_pin
           ,quan::stm32::gpio::mode::input
           ,quan::stm32::gpio::pupd::pull_up
        >();
        quan::stm32::apply<
           video_in_tim3_hsync_pin
           ,quan::stm32::gpio::mode::input
           ,quan::stm32::gpio::pupd::pull_up
        >();
   }

   void video_analog_input_setup()
   {
      quan::stm32::module_enable<video_adc_pin::port_type>();
      quan::stm32::apply<
         video_adc_pin
         ,quan::stm32::gpio::mode::analog
         ,quan::stm32::gpio::pupd::none
      >();
   }

   constexpr uint32_t gpioa_unused[] ={
      0,1,4,5,8,9,10,11,12,13,14 
   };
   constexpr uint32_t gpiob_unused[] ={
      1,3,4,5,6,7,8,9,10,11
   };
   constexpr uint32_t gpioc_unused[] ={
      0,1,3,4,5,9,12,14,15
   };

   // setup unused ports as inputs with pulldown
   template <typename Port, int N>
   void setup_unused_pins(uint32_t const (& pin_array)[N])
   {
      quan::stm32::module_enable<Port>();
      uint32_t moder_and_mask = 0xFFFFFFFF;
      uint32_t pupdr_and_mask = 0xFFFFFFFF;
      uint32_t pupdr_or_mask = 0;
      for ( auto pin : pin_array){
         uint32_t const pos = 2U * pin;
         //-------- 0b00 for input
         moder_and_mask &=  ~(0b11U << pos);
         //-------- 0b10 for pulldown
         pupdr_and_mask &= ~(0b01 << pos);
         pupdr_or_mask  |=  (0b10 << pos);
      }
      Port::get()->moder &= moder_and_mask;
      Port::get()->pupdr =
      ( Port::get()->pupdr & pupdr_and_mask ) | pupdr_or_mask;
   }

   void setup_unused_pins()
   {
      setup_unused_pins<quan::stm32::gpioa>(gpioa_unused);
      setup_unused_pins<quan::stm32::gpiob>(gpiob_unused);
      setup_unused_pins<quan::stm32::gpioc>(gpioc_unused);
   }

}//namespace

void video_cfg::setup()
{
     spi_clock::setup();
     columns::setup();
     rows::setup();
}

void video_cfg::takedown()
{
    spi_clock::takedown();
    rows::takedown();
    columns::takedown();
}
 
#if (defined QUAN_OSD_TELEM_TRANSMITTER)
   void setup_telemetry_transmitter_task();
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
   void setup_telemetry_receiver_task();
#endif
   void setup_draw_task();
//void av_telem_setup();
namespace detail{
   
   void setup_leds();
   void video_palette_dac_setup();
   void external_pixel_dma_setup();
   void external_spi_setup();
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
   void sync_sep_setup(osd_state::state_t state);
   void sync_sep_enable();
#endif

  void external_spi_take_down();
  void sync_sep_takedown();
}
namespace detail{

   void external_video_setup()
   {
        detail::external_spi_setup();
        detail::external_pixel_dma_setup();
        hsync_setup();
   #if ! defined QUAN_OSD_SOFTWARE_SYNCSEP
        vsync_setup();
        odd_even_setup();
   #endif
   #if (defined QUAN_OSD_TELEM_TRANSMITTER)
        setup_telemetry_transmitter_task();
   #endif
   #if (defined QUAN_OSD_TELEM_RECEIVER)
        setup_telemetry_receiver_task();
   #endif
        video_cfg::setup();
        video_buffers::init();
   #if defined QUAN_OSD_SOFTWARE_SYNCSEP
        detail::sync_sep_setup(osd_state::external_video);
        detail::sync_sep_enable();
   #endif
     
   }

  void external_video_take_down()
  {
    external_spi_take_down();
    hsync_take_down();
    video_buffers::init();
    video_cfg::takedown();
    sync_sep_takedown();
    
  }
}

void osd_setup()
{
  // once
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  detail::setup_leds();
  setup_unused_pins();
  video_analog_input_setup();
  detail::video_palette_dac_setup();
  
  osd_state::set(osd_state::external_video);
}
