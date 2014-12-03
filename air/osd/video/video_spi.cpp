
#include <stm32f4xx.h>
#include "../resources.hpp"
#include <quan/stm32/rcc.hpp>

void spi_setup()
{
   // rcc
  // black on spi2, white on spi3
   quan::stm32::module_enable<video_mux_out_black_sck::port_type>();
   quan::stm32::apply<
      video_mux_out_black_sck
      ,quan::stm32::gpio::mode::af5
      ,quan::stm32::gpio::pupd::pull_down // init clock low
   >();

   quan::stm32::module_enable<video_mux_out_black_miso::port_type>();
   quan::stm32::apply<
      video_mux_out_black_miso
      ,quan::stm32::gpio::mode::af5
#if QUAN_OSD_BOARD_TYPE == 1
      ,quan::stm32::gpio::otype::open_drain
#else
    #if  (QUAN_OSD_BOARD_TYPE == 2) || (QUAN_OSD_BOARD_TYPE == 3) || (QUAN_OSD_BOARD_TYPE == 4)
      ,quan::stm32::gpio::otype::push_pull
    #else
      #error undefined board type
    #endif
#endif
      ,quan::stm32::gpio::pupd::none
      ,quan::stm32::gpio::ospeed::fast
      ,quan::stm32::gpio::ostate::high
   >();

   quan::stm32::module_enable<video_mux_out_white_sck::port_type>();
   quan::stm32::apply<
      video_mux_out_white_sck
      ,quan::stm32::gpio::mode::af6
      ,quan::stm32::gpio::pupd::pull_down // init clock low
   >();

   quan::stm32::module_enable<video_mux_out_white_miso::port_type>();
   quan::stm32::apply<
      video_mux_out_white_miso
      ,quan::stm32::gpio::mode::af6
#if QUAN_OSD_BOARD_TYPE == 1
      ,quan::stm32::gpio::otype::open_drain
#else
     #if  (QUAN_OSD_BOARD_TYPE == 2) || (QUAN_OSD_BOARD_TYPE == 3) || (QUAN_OSD_BOARD_TYPE == 4)
      ,quan::stm32::gpio::otype::push_pull
    #else
      #error undefined board type
    #endif
#endif
      ,quan::stm32::gpio::pupd::none
      ,quan::stm32::gpio::ospeed::fast
      ,quan::stm32::gpio::ostate::high
   >();

   //spi_ll_setup();
}