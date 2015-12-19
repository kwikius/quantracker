#ifndef QUANTRACKER_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_RESOURCES_HPP_INCLUDED

/*
 Copyright (c) 2013 Andy Little 

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

#include <quan/stm32/usart.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/i2c_port.hpp>

//timers
// tim1 for pixel_clk
// TIM2 32 bit with PWM
// TIM3 16 bit timer with QDRT  
// however also need 3 more for osd 

typedef quan::stm32::tim2 main_loop_timer;
typedef quan::stm32::tim3 azimuth_qdrt_counter;
//########### only if FSK //#################
typedef quan::stm32::tim4 fsk_filter_timer;
//###########################################

 /*
  using loop_timer  (tim2)
  loop_timer.CC1 used for azimuth motor PWM
  loop_timer CC2 used for elevation servo pwm
  loop_timer.CC3 used for FrSky RcTx Pulses
  loop_timer.CC4 used to gen interrupt to do calc b4 looptimer overrflow
*/

// coudl use pa0 for usart4 tx if removed jumper
// need to move elev servo pwm from pa1 for usart4 rx
typedef quan::mcu::pin<quan::stm32::gpioa,0>    user_switch_pin;  

typedef quan::mcu::pin<quan::stm32::gpioa,1>    elev_servo_pwm_out_pin;                // SF: TIM2_CH2:AF1

//###############use for serial port from air_osd_rx
typedef quan::mcu::pin<quan::stm32::gpioa,2>   air_osd_tx_out_pin;      // USART2_TX 

typedef quan::mcu::pin<quan::stm32::gpioa,3>   air_osd_rx_in_pin;        // USART2_RX
//#############################################################
// keep internal DAC's
//on pa4 / pa5 free

/*
// pa6 free SPI1_MISO/ 
TIM8_BKIN/ 
TIM13_CH1/ 
DCMI_PIXCLK/ 
TIM3_CH1/ 
TIM1_BKIN/ 
ADC12_IN6 
*/
// pa 7 free

typedef quan::mcu::pin<quan::stm32::gpioa,8>    not_azimuth_motor_direction_out_pin;   //( H-bridge blue wire in1, in4)
// cant swap this port with av_telem in as uses higher speed bus and 1200 baud n/a
// but would be nice as has (max)15 k pullup pull down rather than (max)50 k
// requires removal of Discovery cap C49
// output only enabled if pc0 is made output lo, so should be ok if charged once and left
// or just make pin input and dont use the alternate function part.
/*
// pa9 free but has LED on it
// may require removal of Discovery R59 which is a link to the micro usb connector ID pin
USART1_TX/ 
TIM1_CH2/ 
I2C3_SMBA/ 
DCMI_D0/ 
OTG_FS_VBUS 
*/
// change to av_video_telem_in
// Actually looks like may be ok 15 k pullup downs
typedef quan::mcu::pin<quan::stm32::gpioa,10>   av_video_rxi; // USART1_RX  fast 10.5 Mbps

//pb11 free
//pb12 free
//pb13 free ( used on OSD )
//pb14 free (* used on OSD)
typedef quan::mcu::pin<quan::stm32::gpioa,15>   azimuth_motor_pwm_out_pin;             // SF: TIM2_CH1:AF1

typedef quan::mcu::pin<quan::stm32::gpiob,0>    azimuth_motor_direction_out_pin;       // (H-bridge white in2,in3)
typedef quan::mcu::pin<quan::stm32::gpiob,1>    mag_rdy_exti_pin;  // mag new conv ready
//typedef quan::mcu::pin<quan::stm32::gpiob,2>      // UNUSED (BOOT1)

// PB3  = traceSWO but unused (reserved)  in practise can be // SPI3_SCK AF6
typedef quan::mcu::pin<quan::stm32::gpiob,3>    white_pixel_sck; // SPI3_SCK AF6
typedef quan::mcu::pin<quan::stm32::gpiob,4>    white_pixel_miso;      

typedef quan::mcu::pin<quan::stm32::gpiob,5>    azimuth_encoder_a_pin; // SF: TIM3_CH2:AF2  ( encoder yellow 6th from left) 

// HMC5883
typedef quan::mcu::pin<quan::stm32::gpiob,6>    i2c1_scl;   // already connected to SDL on Discovery  
 
//TIM4 CH2    
// or av_video_rxi    
typedef quan::mcu::pin<quan::stm32::gpiob,7>    av_telem_fsk_demod_out;  // AV audio telem data output
// pb8 free
typedef quan::mcu::pin<quan::stm32::gpiob,9>    i2c1_sda;  // already connected to SDA on Discovery
// pb11 free ( used by OSD)
// pb12 free  ( used by OSD)

typedef quan::mcu::pin<quan::stm32::gpiob,13> black_pixel_sck; // SPI2_SCK AF5

// could also use PC2 as SPI2_MISO
typedef quan::mcu::pin<quan::stm32::gpiob,14> black_pixel_miso; // SPI2_MISO AF5

// pb15  (used by OSD) TIM12_CH2 various other timers
// pc0  attached to STMPS2141STR
// N.B not 5v tolerant in analog mode
typedef quan::mcu::pin<quan::stm32::gpioc,1>    av_telem_raw_fsk_in_plus; // ADC123_IN11 
typedef quan::mcu::pin<quan::stm32::gpioc,2>    av_telem_raw_fsk_in_minus; // ADC123_IN12

typedef quan::mcu::pin<quan::stm32::gpioc,4>    azimuth_motor_v_A;//(ADC12_IN14) for reading azimuth motor speed
typedef quan::mcu::pin<quan::stm32::gpioc,5>    azimuth_motor_v_B;//(ADC12_IN15) for reading azimuth motor speed
typedef quan::mcu::pin<quan::stm32::gpioc,6>    azimuth_encoder_b_pin; // SF: TIM3_CH1:AF2 ( encoder white, 5th from left)

// pc7 -- usart6 rx ( also used for  MCLK input to CS43L22, so should be ok to use except cant be driven more
// than 3.5 V 
// pc8 free
// change to GPS?
// define but dont use,poss set it to open drain?
// or input
// (Used by CS43L22 SCLK i/O. Think its set as an input on that ic default
// so should be ok as output, but limited to 3.5 v!
// can get rid of this now
// pc9 avail
// pc10 avail
typedef quan::mcu::pin<quan::stm32::gpioc,11>   av_telem_rx_pin;
// NB -----------------------------Except PD2 those below are NA on 64 pin part!!! -------------------
// seek alternate pins for usart2 and usart3
// SDIN to CS43L22. Therefore OK to use as uart output
// but  5V ok for CS43L22 ?
typedef quan::mcu::pin<quan::stm32::gpioc,12>    gps_txo;  //UART5_TX
// pd1 for osd
// PD2 can be uart 5 rx
typedef quan::mcu::pin<quan::stm32::gpiod,2>    gps_rxi; // UART5_RX
// PD3 for osd
// N.B TX I think is knacked unless mod board
typedef quan::mcu::pin<quan::stm32::gpiod,5>    frsky_txo_pin;                         // SF:USART2_TX:AF7(!!!remove R50 on Discovery!!!)
typedef quan::mcu::pin<quan::stm32::gpiod,6>    frsky_rxi_pin;                         // SF:USART2_RX:AF7
// PD7 for osd
typedef quan::mcu::pin<quan::stm32::gpiod,8>    rctx_txo_pin;                          // SF:USART3_TX:AF7
typedef quan::mcu::pin<quan::stm32::gpiod,9>    rctx_rxi_pin;                          // SF:UASRT3_RX:AF7

// pd10 free
// pd11 free 

typedef quan::mcu::pin<quan::stm32::gpiod,12>   green_led_pin;   // green led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,13>   orange_led_pin;       // orange led on Discovery
typedef quan::mcu::pin<quan::stm32::gpiod,14>   red_led_pin;  // red led on Discover
typedef quan::mcu::pin<quan::stm32::gpiod,15>   blue_led_pin;       // blue led on Discover

//pe3 free
// pe4 free

typedef quan::mcu::pin<quan::stm32::gpioe,5> hsync_falling_pin; // TIM9_CH1
typedef quan::mcu::pin<quan::stm32::gpioe,6> hsync_rising_pin; // TIM9_CH2

typedef quan::mcu::pin<quan::stm32::gpioe,9>  pixel_spi_clock ; // TIM1_CH1

typedef quan::stm32::i2c_port<quan::stm32::i2c1,i2c1_scl,i2c1_sda> i2c_mag_port;

//might be better for telem as is fast!
typedef quan::stm32::usart1 gps_usart;   //tx & rx dependent on GPS freq
typedef quan::stm32::usart2 air_osd_serial_port;  // tx & rx 9600( only needs to be rx though!)
typedef quan::stm32::usart3 sliprings_usart;  // tx & rx via sliprings 9600
// cant use usart1 or usart6 as cant support (low) 1200 baud
typedef quan::stm32::uart4  av_telem_uart; // 1200 baud rx only
typedef quan::stm32::uart5  free_usart_rx; // rx only
//usart 6 fo av video_telem

struct interrupt_priority{
   static constexpr uint32_t systick_timer = 15;
   static constexpr uint32_t exti_mag_rdy = 14;
   static constexpr uint32_t airosd_serial_port= 13;
   static constexpr uint32_t sliprings_serial_port = 13;
   static constexpr uint32_t av_fsk_serial_port = 12;
   static constexpr uint32_t i2c_mag_evt  = 10;
   static constexpr uint32_t loop_timer = 9;
   static constexpr uint32_t fsk_adc = 8;
   
};

// ADC's fsk input uses ADC1 and ADC2

#endif // QUANTRACKER_RESOURCES_HPP_INCLUDED
