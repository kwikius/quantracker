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
#include <quan/stm32/rcc.hpp>

namespace detail{
   // just setup the spi pins here
   // actual spi setup is done variously
   void spi_setup()
   {
      
      // rcc
     // black on spi2, white on spi3
      quan::stm32::module_enable<video_mux_out_black_sck::port_type>();
      quan::stm32::apply<
         video_mux_out_black_sck   // same on all boards
         ,quan::stm32::gpio::mode::af5
         ,quan::stm32::gpio::pupd::pull_down // init clock low
      >();

      quan::stm32::module_enable<video_mux_out_black_miso::port_type>();
      quan::stm32::apply<
         video_mux_out_black_miso  // PB14 or PC2 on boardtype 4
         ,quan::stm32::gpio::mode::af5  // same for both pins
   #if QUAN_OSD_BOARD_TYPE == 1
         ,quan::stm32::gpio::otype::open_drain
   #else
       #if  (QUAN_OSD_BOARD_TYPE == 2) || (QUAN_OSD_BOARD_TYPE == 3) || (QUAN_OSD_BOARD_TYPE == 4)
         ,quan::stm32::gpio::otype::push_pull
       #else
         #error undefined board type
       #endif
   #endif
   #if QUAN_OSD_BOARD_TYPE == 1
         ,quan::stm32::gpio::pupd::none
   #else
         ,quan::stm32::gpio::pupd::pull_up
   #endif
         ,quan::stm32::gpio::ospeed::fast
         ,quan::stm32::gpio::ostate::high
      >();

      quan::stm32::module_enable<video_mux_out_white_sck::port_type>();
      quan::stm32::apply<
         video_mux_out_white_sck   //PB3 or PC10 on baordtype 4
         ,quan::stm32::gpio::mode::af6  // same for both pins
         ,quan::stm32::gpio::pupd::pull_down // init clock low
      >();

      quan::stm32::module_enable<video_mux_out_white_miso::port_type>();
      quan::stm32::apply<
         video_mux_out_white_miso  //PB4 or PC11 on boardtype 4
         ,quan::stm32::gpio::mode::af6 // same for both pins
   #if QUAN_OSD_BOARD_TYPE == 1
         ,quan::stm32::gpio::otype::open_drain
   #else
        #if  (QUAN_OSD_BOARD_TYPE == 2) || (QUAN_OSD_BOARD_TYPE == 3) || (QUAN_OSD_BOARD_TYPE == 4)
         ,quan::stm32::gpio::otype::push_pull
       #else
         #error undefined board type
       #endif
   #endif
   // prob shoulnt be for Boardtype1 since goes to 5V
   #if QUAN_OSD_BOARD_TYPE == 1
         ,quan::stm32::gpio::pupd::none
   #else
         ,quan::stm32::gpio::pupd::pull_up
   #endif
         ,quan::stm32::gpio::ospeed::fast
         ,quan::stm32::gpio::ostate::high
      >();

   }
} // detail