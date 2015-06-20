#ifndef QUANTRACKER_GROUND_TRACKER_V2_RESOURCES_OSD_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_RESOURCES_OSD_RESOURCES_HPP_INCLUDED

/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korhonen(Openpilot.org)
   taulabs ( taulabs.com) 
   brainFPV ( brainfpv.com)
   Thomas Oldbury (super-osd.com)

 Much Thanks also to Andrew Fernie

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

//#include <quan/voltage.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>

/*
 For the V1 and V2.1 boards
 These resources are reserved for exclusive use by the OSD
  (except heartbeat_led_pin which is just wired on the board
   so not on breakout pins
*/

//timers used
typedef quan::stm32::tim2                       video_columns_gate_timer;
typedef quan::stm32::tim3                       video_rows_line_counter;

typedef quan::stm32::tim9                       spi_clock_timer;
typedef quan::stm32::tim10                      video_level_dac_irq_timer;
typedef quan::stm32::tim12                      sync_sep_timer;
// usarts used (for vsync telemetry )
typedef quan::stm32::usart6                     av_telem_usart;
// SPI's
typedef quan::stm32::spi2                       video_mux_out_black_spi;
typedef quan::stm32::spi3                       video_mux_out_white_spi;
// Pins used
typedef quan::mcu::pin<quan::stm32::gpioa,2>    video_spi_clock;// TIM9_CH1
typedef quan::mcu::pin<quan::stm32::gpioa,6>    av_dac_nsync; // software bitbanged
typedef quan::mcu::pin<quan::stm32::gpioa,7>    av_dac_data; // software bitbanged
typedef quan::mcu::pin<quan::stm32::gpioa,15>   video_in_tim2_hsync_pin ; // TIM2_CH1 ( also TIM2_ETR)
typedef quan::mcu::pin<quan::stm32::gpiob,0>    video_adc_pin ; // ADC12_IN8
typedef quan::mcu::pin<quan::stm32::gpiob,12>   heartbeat_led_pin; 
typedef quan::mcu::pin<quan::stm32::gpiob,13>   video_mux_out_black_sck; // SPI2_SCK AF5
typedef quan::mcu::pin<quan::stm32::gpiob,14>   video_in_hsync_first_edge_pin; // TIM12_CH1
typedef quan::mcu::pin<quan::stm32::gpiob,15>   video_in_hsync_second_edge_pin; // TIM12_CH2
typedef quan::mcu::pin<quan::stm32::gpioc,2>    video_mux_out_black_miso; // SPI2_MISO AF5
typedef quan::mcu::pin<quan::stm32::gpioc,6>    av_telem_tx; // USART6_TX
typedef quan::mcu::pin<quan::stm32::gpioc,7>    av_telem_rx; // USART6_RX
typedef quan::mcu::pin<quan::stm32::gpioc,8>    av_dac_clk; // software bitbanged
typedef quan::mcu::pin<quan::stm32::gpioc,10>   video_mux_out_white_sck; // SPI3_SCK 
typedef quan::mcu::pin<quan::stm32::gpioc,11>   video_mux_out_white_miso; // SPI3_MISO
typedef quan::mcu::pin<quan::stm32::gpiod,2>    video_in_tim3_hsync_pin; // TIM3_ETR


#endif // QUANTRACKER_GROUND_TRACKER_V2_RESOURCES_OSD_RESOURCES_HPP_INCLUDED
