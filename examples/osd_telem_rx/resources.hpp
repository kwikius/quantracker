#ifndef QUANTRACKER_AIR_OSD_PROCESSORS_BOARD_TYPE4_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_PROCESSORS_BOARD_TYPE4_HPP_INCLUDED

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
#include <quan/stm32/freertos/freertos_usart_task_tx_only.hpp>
#include <quan/stm32/serial_port_lite.hpp>
#include <quan/voltage.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>

//timers used
typedef quan::stm32::tim2                       video_columns_gate_timer;
typedef quan::stm32::tim3                       video_rows_line_counter;
typedef quan::stm32::tim6                       fsk_dac_timer;
typedef quan::stm32::tim7                       dac2_timer;
typedef quan::stm32::tim9                       spi_clock_timer;
typedef quan::stm32::tim10                      video_level_dac_irq_timer;
typedef quan::stm32::tim12                      sync_sep_timer;

/*Timers potentially avail for expansion some multiplexed with other functions ( with i/o)
 N.B. Timer1 CH2 could go out on PA9 (currently Mavlink TX via digitally isolated port
 which might make a nice digitally isolated ESC pwm rc output
 PA10 (Mavlink Rx)  could be used for PPMSUM capture on TIM1 CH3 
TIM1_CH1 
TIM1_CH2-3( on 100 pin part)
TIM1_CH4
TIM4_CH1
TIM4_CH2
TIM4_CH3-4 ( on 100 pin part)
TIM4_CH2
TIM5_CH1
TIM5_CH2
TIM8_CH4
TIM10_CH1 
TIM11_CH1 

/expansion 100 pin io
 10 for timers
 6 for usarts
 3 for spi
 6 for i2c
 6 for ADC
 8 gp port
 2 can
-------------------
 total 43 + power + gnd
 abs min 42 -> 48/4 -> 4 * 13 ( 2 rows of 13 * 2) 52
*/
typedef quan::stm32::usart1                    mavlink_usart;
// usart2 avail for expansion on 100 pin part
// usart4 avail for expansion on all parts
typedef quan::stm32::usart3                    frsky_usart; // maybe inverted but not on f4
// uart5 txo avail on all parts
typedef quan::stm32::usart6                    av_telem_usart;
// spi
// SPI1 avail for expansion
typedef quan::stm32::spi2                      video_mux_out_black_spi;
typedef quan::stm32::spi3                      video_mux_out_white_spi;
// I2C1 I2C2 and I2C3 avail for expansion
//----PORTA---------------------------------------
//typedef quan::mcu::pin<quan::stm32::gpioa,0>    usart4_tx ;// USART4_TX
//typedef quan::mcu::pin<quan::stm32::gpioa,1>    usart4_rx; // USART4_RX
typedef quan::mcu::pin<quan::stm32::gpioa,2>    video_spi_clock;// TIM9_CH1
typedef quan::mcu::pin<quan::stm32::gpioa,3>    telem_cmp_enable_pin; // TIM2_CH4
typedef quan::mcu::pin<quan::stm32::gpioa,4>    fsk_dac_out_pin; // NOT MOVABLE
typedef quan::mcu::pin<quan::stm32::gpioa,5>    dac2_out_pin; // NOT MOVABLE
#if !(defined QUAN_DISCOVERY)
typedef quan::mcu::pin<quan::stm32::gpioa,6>    av_dac_nsync; // software no af
typedef quan::mcu::pin<quan::stm32::gpioa,7>    av_dac_data; // softawre no af
#endif
//typedef quan::mcu::pin<quan::stm32::gpioa,8>    i2c3_scl ;
typedef quan::mcu::pin<quan::stm32::gpioa,9>     mavlink_txo_pin; // USART1_TX
typedef quan::mcu::pin<quan::stm32::gpioa,10>    mavlink_rxi_pin; // USART1_RX
//typedef quan::mcu::pin<quan::stm32::gpioa,11>    can1_tx;
//typedef quan::mcu::pin<quan::stm32::gpioa,12>    can1_rx;
typedef quan::mcu::pin<quan::stm32::gpioa,13>    swdio;
typedef quan::mcu::pin<quan::stm32::gpioa,14>    swdclk;
typedef quan::mcu::pin<quan::stm32::gpioa,15>    video_in_tim2_hsync_pin ; // TIM2_CH1 ( also TIM2_ETR)
//----PORTB---------------------------------------------
typedef quan::mcu::pin<quan::stm32::gpiob,0>      video_adc_pin ; // ADC12_IN8
typedef quan::mcu::pin<quan::stm32::gpiob,1>      vin_voltage_pin ;//adc1 ADC12_IN9 
typedef quan::mcu::pin<quan::stm32::gpiob,2>      boot1_pin;
//typedef quan::mcu::pin<quan::stm32::gpiob,3>    spi1_sck; // NOT MOVABLE
//typedef quan::mcu::pin<quan::stm32::gpiob,4>    spi1_miso; // or SPI3_MISO
//typedef quan::mcu::pin<quan::stm32::gpiob,5>    spi1_mosi;
//typedef quan::mcu::pin<quan::stm32::gpiob,6>    i2c1_scl; 
//typedef quan::mcu::pin<quan::stm32::gpiob,7>    i2c1_sda; 
//typedef quan::mcu::pin<quan::stm32::gpiob,8>    tim10_ch1; 
//typedef quan::mcu::pin<quan::stm32::gpiob,9>    tim11_ch1; 
typedef quan::mcu::pin<quan::stm32::gpiob,10>   frsky_txo_pin; 
typedef quan::mcu::pin<quan::stm32::gpiob,11>   frsky_rxi_pin; 
#if !(defined QUAN_DISCOVERY)
typedef quan::mcu::pin<quan::stm32::gpiob,12>   heartbeat_led_pin;
#endif
typedef quan::mcu::pin<quan::stm32::gpiob,13>   video_mux_out_black_sck; // SPI2_SCK AF5
typedef quan::mcu::pin<quan::stm32::gpiob,14>   video_in_hsync_first_edge_pin; // TIM12_CH1
typedef quan::mcu::pin<quan::stm32::gpiob,15>   video_in_hsync_second_edge_pin; // TIM12_CH2
//-----PORTC---------------------------------------
//typedef quan::mcu::pin<quan::stm32::gpioc,0> adc2; //ADC123_IN10
//typedef quan::mcu::pin<quan::stm32::gpioc,1> adc3; //ADC123_IN11
typedef quan::mcu::pin<quan::stm32::gpioc,2> video_mux_out_black_miso; // SPI2_MISO AF5
//typedef quan::mcu::pin<quan::stm32::gpioc,3> adc4; //ADC123_IN13
//typedef quan::mcu::pin<quan::stm32::gpioc,4> adc5; //ADC123_IN14
//typedef quan::mcu::pin<quan::stm32::gpioc,5> adc6; //ADC123_IN15
typedef quan::mcu::pin<quan::stm32::gpioc,6> av_telem_tx; // USART6_TX
typedef quan::mcu::pin<quan::stm32::gpioc,7> av_telem_rx; // USART6_RX
#if !(defined QUAN_DISCOVERY)
typedef quan::mcu::pin<quan::stm32::gpioc,8> av_dac_clk; // software no af
#endif
//typedef quan::mcu::pin<quan::stm32::gpioc,9> i2c3_sda ;
typedef quan::mcu::pin<quan::stm32::gpioc,10> video_mux_out_white_sck; // SPI3_SCK 
typedef quan::mcu::pin<quan::stm32::gpioc,11> video_mux_out_white_miso; // SPI3_MISO
//typedef quan::mcu::pin<quan::stm32::gpioc,12> mcu_pc12_pin ; // UART5_TXO
//############Limited output current ###########################
typedef quan::mcu::pin<quan::stm32::gpioc,13> frsky_txo_sign_pin;
//typedef quan::mcu::pin<quan::stm32::gpioc,14> osc32_in;;
//typedef quan::mcu::pin<quan::stm32::gpioc,15> osc32_out;
//############Limited output current ############################
// NA on 64 pin part except PD2 PH0 PH1
//---------PORTD---------------------------------------------------------------------
/*
PD0 GP no useful AF no ADC

*/
#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpiod,1>     av_dac_nsync; // software no af
#endif
typedef quan::mcu::pin<quan::stm32::gpiod,2>     video_in_tim3_hsync_pin; // TIM3_ETR
#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpiod,3>     av_dac_data; // software no af
#endif
/*
PD4 GP No useful AF no ADC (NC)
*/
//typedef quan::mcu::pin<quan::stm32::gpiod,5>     usart2_tx;
//typedef quan::mcu::pin<quan::stm32::gpiod,6>     usart2_rx;
/*
PD7 GP no AF no ADC (NC)
*/
#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpiod,7>     av_dac_clk; // software no af
#endif
//typedef quan::mcu::pin<quan::stm32::gpiod,8>    usart3_tx;
//typedef quan::mcu::pin<quan::stm32::gpiod,9>    usart3_rx;
/*
PD10 GP no AF no ADC (NC)
PD11 GP no AF no ADC (NC)
*/
//typedef quan::mcu::pin<quan::stm32::gpiod,12>   tim4_ch1;   // No other AF
//typedef quan::mcu::pin<quan::stm32::gpiod,13>   tim4_ch2;   // No other AF   
//typedef quan::mcu::pin<quan::stm32::gpiod,14>   tim4_ch3;  // No other AF
#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpiod,12>   green_led_pin;   // green led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,13>   orange_led_pin;       // orange led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,14>   heartbeat_led_pin;
typedef quan::mcu::pin<quan::stm32::gpiod,15>   blue_led_pin;       // blue led on Discovery
#endif

//typedef quan::mcu::pin<quan::stm32::gpiod,15>   tim4_ch4;    // No other AF 

//----------PORTE--------------------------
/* 8 bit software port 
PE0 TIM4_ETR NO ADC
PE1 GP NO AF NO ADC
PE2 GP NO AF NO ADC
PE3 GP NO AF NO ADC
PE4 GP NO AF NO ADC
//#####################################
// could multiplex sync_sep timer with spi clock via these 2 pins?
/ but only on 100 pin part
PE5 TIM9_CH1 NO ADC
PE6 TIM9_CH2 NO ADC
//################################
PE7 TIM1_ETR
PE8 GP NO ADC (NC)
*/
//typedef quan::mcu::pin<quan::stm32::gpioe,9> tim1_ch1;
/*
PE10 NO ADC (NC)
*/
//typedef quan::mcu::pin<quan::stm32::gpioe,11> tim1_ch2;
/*
PE12 NO ADC (NC)
*/
//typedef quan::mcu::pin<quan::stm32::gpioe,13> tim1_ch3;
//typedef quan::mcu::pin<quan::stm32::gpioe,14> tim1_ch4;
/*
PE15 NO ADC (NC)
*/
//----------------PORTH---------------
/*
PH0 OSC_IN
PH1 OSC_OUT
*/
typedef quan::stm32::freertos::usart_tx_rx_task<
   mavlink_usart,
   100,100, 
   mavlink_txo_pin,mavlink_rxi_pin,
   uint8_t
> mavlink_tx_rx_task;

typedef quan::stm32::serial_port_lite<
      mavlink_usart,
      mavlink_txo_pin,
      mavlink_rxi_pin
> flash_menu_sp;

typedef quan::stm32::freertos::usart_tx_rx_task<
   frsky_usart,
   10,2, 
   frsky_txo_pin,frsky_rxi_pin,
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
#if defined QUAN_OSD_TELEM_TRANSMITTER

   static constexpr uint32_t vsync_telem_tx =( tskIDLE_PRIORITY + 3UL ) ;
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
   static constexpr uint32_t vsync_telem_rx =( tskIDLE_PRIORITY + 3UL ) ;
#endif
    static constexpr uint32_t draw =( tskIDLE_PRIORITY + 2UL ) ;

// these prob dont belong here should prob be in user namespace
 // somehow to get priority
   static constexpr uint32_t fsk = ( tskIDLE_PRIORITY + 1UL );
   static constexpr uint32_t mavlink = ( tskIDLE_PRIORITY + 4UL );
   static constexpr uint32_t frsky = ( tskIDLE_PRIORITY + 3UL );

   static constexpr uint32_t heartbeat = ( tskIDLE_PRIORITY + 1UL );

};

#endif // QUANTRACKER_AIR_OSD_PROCESSORS_BOARD_TYPE4_HPP_INCLUDED
