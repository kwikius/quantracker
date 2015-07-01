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

#include "video_cfg.hpp"
#include "video_buffer.hpp"
#include "../resources.hpp"

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

#if !defined QUAN_OSD_SOFTWARE_SYNCSEP
void vsync_setup()
{
     quan::stm32::module_enable<video_in_vsync_pin::port_type>();
     // TIM3_CH1
     quan::stm32::apply<
        video_in_vsync_pin,
        quan::stm32::gpio::mode::af2
     >();
}

void odd_even_setup()
{
     quan::stm32::module_enable<video_in_odd_even_pin::port_type>();

     quan::stm32::apply<
        video_in_odd_even_pin,
        quan::stm32::gpio::mode::input,
        quan::stm32::gpio::pupd::pull_up
     >();
}
#endif

}//namespace


void video_cfg::setup()
{
     spi_clock::setup();
     columns::setup();
     rows::setup();
}
 
#if (defined QUAN_OSD_TELEM_TRANSMITTER)
  void vsync_telem_tx_task_setup();
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
   void setup_telemetry_receiver_task();
#endif
void av_telem_setup();
void pixel_dma_setup();
void spi_setup();
 
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
namespace detail{
   void sync_sep_enable();
}
void sync_sep_setup();
#endif

void Dac_setup();
void video_setup()
{
     spi_setup();
     pixel_dma_setup();
     hsync_setup();
#if ! defined QUAN_OSD_SOFTWARE_SYNCSEP
     vsync_setup();
     odd_even_setup();
#endif
#if (defined QUAN_OSD_TELEM_TRANSMITTER)
     vsync_telem_tx_task_setup();
#endif
#if (defined QUAN_OSD_TELEM_RECEIVER)
     setup_telemetry_receiver_task();
#endif
     video_cfg::setup();
     video_buffers::init();
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
     sync_sep_setup();
     detail::sync_sep_enable();
#endif
     Dac_setup();
}
