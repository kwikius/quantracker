#ifndef QUANTRACKER_AIR_OSD_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_RESOURCES_HPP_INCLUDED

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

#include <quan/stm32/freertos/freertos_usart_task.hpp>
#include <quan/voltage.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>

#if (QUAN_OSD_BOARD_TYPE == 4)
#include "processor/board_type4.hpp"
#else
//timers
typedef quan::stm32::tim1                       spi_clock_timer;
typedef quan::stm32::tim2                       video_columns_gate_timer;
typedef quan::stm32::tim3                       video_rows_line_counter;
typedef quan::stm32::tim6                       fsk_dac_timer;
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
/*
   2 channel timer us channels 1 and 2 for getting sync pulse length
   and length of a line etc
*/
typedef quan::stm32::tim9                       sync_sep_timer;
#endif
/*
   To avoid use of SPI loading the video color dac is done
   with an interrupt of the timer
*/
typedef quan::stm32::tim10                      video_level_dac_irq_timer;
/*
 Not used currently for transmitter,but wil be eventually
 to transmit the telem at start of video frame
*/
typedef quan::stm32::usart1                     av_telemetry_usart;
typedef quan::stm32::usart2                     frsky_usart; // maybe inverted but not on f4
typedef quan::stm32::usart3                     posdata_usart;
// usart4 avail for expansion
// usart6 avail for expansion
//spi
// SPI1 avail for expansion
typedef quan::stm32::spi2                       video_mux_out_black_spi;
typedef quan::stm32::spi3                       video_mux_out_white_spi;
// I2C1 and I2C3 avail for expansion
//----PORTA---------------------------------------
#if !defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpioa,0>    usart4_tx ;
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
typedef quan::mcu::pin<quan::stm32::gpioa,1>    telem_cmp_enable_pin; // TIM2_CH2
#else 
//USART4_TX
typedef quan::mcu::pin<quan::stm32::gpioa,1>    usart4_rx; 
#endif

#if !(defined QUAN_OSD_SOFTWARE_SYNCSEP)
#error need to sort pins here

 typedef quan::mcu::pin<quan::stm32::gpioa,2>    video_in_odd_even_pin ; // no special requirements!
#else
// POSS move pixel clock here on TIM9
// move frsky_txo_pin to PC12 uart 5
typedef quan::mcu::pin<quan::stm32::gpioa,2>    frsky_txo_pin;  // USART2 TX or TIM5 CH3
#endif
//##############################
//pa3 free frsky rx
// only for compile .. need to do tx only usart for freertos
// TIM5_CH4 
typedef quan::mcu::pin<quan::stm32::gpioa,3>    frsky_rxi_pin_unused;  // not used ... 
// 
//#########################
typedef quan::mcu::pin<quan::stm32::gpioa,4>    fsk_dac_out_pin;
// prob most useful to do audio..
typedef quan::mcu::pin<quan::stm32::gpioa,5>    dac2_out_pin;
/*
PA6 TIM13_CH1 SPI1_MISO
PA7 TIM14_CH1 SPI1_MOSI
*/
#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpioa,8>  video_spi_clock ; // TIM1_CH1
#else
typedef quan::mcu::pin<quan::stm32::gpioa,8>  i2c3_scl ;
#endif
#if !(defined QUAN_DISCOVERY)
typedef quan::mcu::pin<quan::stm32::gpioa,9>    heartbeat_led_pin;
#endif
typedef quan::mcu::pin<quan::stm32::gpioa,10>   frsky_txo_sign_pin;
/*
CAN protocol useful?
PA11 - CAN1 TX
PA12 - CAN1 RX
*/
/*
SWD
PA13    SWDIO
PA14    SWCLK
*/
typedef quan::mcu::pin<quan::stm32::gpioa,15>   video_in_tim2_hsync_pin ; // TIM2_CH1 ( also TIM2_ETR)
//----PORTB---------------------------------------------
/*
PB0  TIM8_CH2
PB1  TIM8_CH3
*/
#define TEST_OUTPUT_PIN_ENABLE 1
typedef quan::mcu::pin<quan::stm32::gpiob,2>    test_output_pin;

#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpiob,3>    video_mux_out_white_sck; // SPI3_SCK AF6
typedef quan::mcu::pin<quan::stm32::gpiob,4>    video_mux_out_white_miso; // SPI3_MISO AF6
#else
// SPI port for expansion
typedef quan::mcu::pin<quan::stm32::gpiob,3> spi1_sck;
typedef quan::mcu::pin<quan::stm32::gpiob,4> spi1_miso;
typedef quan::mcu::pin<quan::stm32::gpiob,5> spi1_mosi;
#endif
#if defined QUAN_OSD_TELEM_TRANSMITTER
typedef quan::mcu::pin<quan::stm32::gpiob,6>    av_video_txo; // USART1 TX
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
typedef quan::mcu::pin<quan::stm32::gpiob,7>    av_video_rxi; // USART1_RX  fast 10.5 Mbps
#else
// I2C port for expansion
typedef quan::mcu::pin<quan::stm32::gpiob,7>    i2c1_sda; 
typedef quan::mcu::pin<quan::stm32::gpiob,8>    i2c1_scl;
#endif
/*
PB9 TIM11_CH1 TIM4_CH4 I2C1_SDA
*/
#if !defined QUAN_DISCOVERY
// also bootloader via usart
typedef quan::mcu::pin<quan::stm32::gpiob,10>     posdata_txo_pin; // SF:USART3_TX:AF7
typedef quan::mcu::pin<quan::stm32::gpiob,11>     posdata_rxi_pin; // SF:USART3_RX:AF7
#endif
/*
PB12 - general purpose - no useful SF
*/
typedef quan::mcu::pin<quan::stm32::gpiob,13>   video_mux_out_black_sck; // SPI2_SCK AF5
typedef quan::mcu::pin<quan::stm32::gpiob,14>   video_mux_out_black_miso; // SPI2_MISO AF5
/*
PB15 TIM8 CH3 TIM12 CH2
*/
//-----PORTC---------------------------------------
/*
PC0 ADC
PC1 ADC
PC2 ADC
PC3 ADC
PC4 ADC
PC5 ADC
*/
#if !(defined QUAN_OSD_SOFTWARE_SYNCSEP)
typedef quan::mcu::pin<quan::stm32::gpioc,6> video_in_vsync_pin ; // TIM3_CH1
#else
typedef quan::mcu::pin<quan::stm32::gpioc,6> usart6_tx;
typedef quan::mcu::pin<quan::stm32::gpioc,7> usart6_rx;
#endif
/*
PC8 TIM8_CH3 
*/
#if ! defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpioc,9> i2c3_sda ;
#endif

#if !defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpioc,10>     video_mux_out_white_sck; // SPI3_SCK 
typedef quan::mcu::pin<quan::stm32::gpioc,11>     video_mux_out_white_miso; // SPI3_MISO
#endif
/*
// could move frsky tx here and get another usart
PC12  UART5_TX
PC13  GP no af
*/
// NA on 64 pin part except PD2 PH0 PH1
//---------PORTD--------------------------------
/*
PD0 GP no useful AF
*/
typedef quan::mcu::pin<quan::stm32::gpiod,1>     av_dac_nsync; // software no af
typedef quan::mcu::pin<quan::stm32::gpiod,2>     video_in_tim3_hsync_pin; // TIM3_ETR
typedef quan::mcu::pin<quan::stm32::gpiod,3>     av_dac_data; // software no af
/*
PD4
PD5
PD6
*/
typedef quan::mcu::pin<quan::stm32::gpiod,7>     av_dac_clk; // software no af
#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpiod,8>     posdata_txo_pin; // SF:USART3_TX:AF7
typedef quan::mcu::pin<quan::stm32::gpiod,9>     posdata_rxi_pin; // SF:USART3_RX:AF7
#endif
typedef posdata_txo_pin debug_txo_pin ;
typedef posdata_rxi_pin debug_rxi_pin;

#if (defined QUAN_DISCOVERY) && (defined QUAN_STM32F4)

typedef quan::mcu::pin<quan::stm32::gpiod,12>   green_led_pin;   // green led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,13>   orange_led_pin;       // orange led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,14>   heartbeat_led_pin;
typedef quan::mcu::pin<quan::stm32::gpiod,15>   blue_led_pin;       // blue led on Discovery
#else
typedef quan::mcu::pin<quan::stm32::gpiod,12>   tim4_ch1;   
typedef quan::mcu::pin<quan::stm32::gpiod,13>   tim4_ch2;     
typedef quan::mcu::pin<quan::stm32::gpiod,14>   tim4_ch3;
typedef quan::mcu::pin<quan::stm32::gpiod,15>   tim4_ch4;    
#endif
//----------PORTE--------------------------
/*
PE0
PE1
PE2
PE3
PE4
*/
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
// need move to other timer on 64 pin
typedef quan::mcu::pin<quan::stm32::gpioe,5> video_in_hsync_first_edge_pin; // TIM9_CH1
typedef quan::mcu::pin<quan::stm32::gpioe,6> video_in_hsync_second_edge_pin; // TIM9_CH2
#endif
/*
PE7
PE8
*/
#if ! defined QUAN_DISCOVERY
//could move to other timer on APB2 bus
typedef quan::mcu::pin<quan::stm32::gpioe,9> video_spi_clock; // TIM1_CH1
#endif
/*
PE10
PE11
PE12
PE13
PE14
PE15
*/
//----------------PORTH---------------

#if (QUAN_OSD_BOARD_TYPE == 2 )  || (QUAN_OSD_BOARD_TYPE == 3 )
void Dac_setup();
// for 8 bit only msbyte of val is used
// code is 00 write to specific reg but dont update
// 1 is write to specific reg and update outputs
// 2 is write tao all registers and update outputs
// 3 is power down outputs
// really only 0 1nd 1 are useful
void Dac_write(uint8_t ch, quan::voltage::V const & vout, uint8_t code);

#endif

typedef quan::stm32::freertos::usart_tx_rx_task<
   posdata_usart,
   2,20, 
   posdata_txo_pin,posdata_rxi_pin,
   uint8_t
> posdata_tx_rx_task;

typedef quan::stm32::freertos::usart_tx_rx_task<
   frsky_usart,
   10,2, 
   frsky_txo_pin,frsky_rxi_pin_unused,
   char
> frsky_tx_rx_task;

// 0- 15 lower numerical is higher logical priority
 // NB anything above certain level is not masked
struct interrupt_priority {
     static constexpr uint32_t video_level = 15;
     static constexpr uint32_t frsky_serial_port= 14;
     static constexpr uint32_t telemetry_input_port = 13;
     static constexpr uint32_t fsk_dac_timer = 12;
     static constexpr uint32_t video = 11;
};

struct task_priority{

   static constexpr uint32_t av_telemetry_tx =( tskIDLE_PRIORITY + 2UL ) ;
   static constexpr uint32_t av_telemetry_tx =( tskIDLE_PRIORITY + 2UL ) ;
   static constexpr uint32_t fsk = ( tskIDLE_PRIORITY + 1UL );
   static constexpr uint32_t mavlink = ( tskIDLE_PRIORITY + 4UL );
   static constexpr uint32_t frsky = ( tskIDLE_PRIORITY + 3UL );
   static constexpr uint32_t draw =( tskIDLE_PRIORITY + 2UL ) ;
   static constexpr uint32_t heartbeat = ( tskIDLE_PRIORITY + 1UL );

};
#endif
#endif // QUANTRACKER_AIR_OSD_RESOURCES_HPP_INCLUDED
