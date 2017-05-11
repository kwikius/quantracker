#ifndef QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED

/*
 Copyright (c) 2017 Andy Little 

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

#include <quan/voltage.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>
//#include <quan/stm32/i2c_port.hpp>
//#include <quan/stm32/freertos/freertos_i2c_task.hpp>
#include <quan/stm32/freertos/freertos_usart_task.hpp>
#include "common_resources.hpp"

/*
non OSD resources used by the tracker
no fsk if want dma for v sampling
remove qdrt would free up some timers etc
also add some leds
push buttons etc

--- pan motor
       motor_direction           PC14
       motor pwm                 PB8
*/
//timers not available - used by osd
/*
typedef quan::stm32::tim2                       video_columns_gate_timer;
typedef quan::stm32::tim3                       video_rows_line_counter;
typedef quan::stm32::tim9                       spi_clock_timer;
typedef quan::stm32::tim12                      sync_sep_timer;
*/

typedef quan::stm32::tim1   pan_motor_timer;

typedef quan::mcu::pin<quan::stm32::gpiob,1>   pan_motor_pwm_out_pin; // TIM1_CH3N
typedef quan::mcu::pin<quan::stm32::gpioc,4>   pan_motor_direction_out_pin;
typedef quan::mcu::pin<quan::stm32::gpioc,5>   pan_motor_not_direction_out_pin;

typedef quan::mcu::pin<quan::stm32::gpioc,0>   pan_motor_emf_adc_pin;
typedef quan::mcu::pin<quan::stm32::gpioc,1>   pan_motor_current_adc_pin;
typedef quan::mcu::pin<quan::stm32::gpioa,11>  elevation_servo_pwm_out_pin;

typedef quan::mcu::pin<quan::stm32::gpioc,14>  user_button_pin;

typedef quan::mcu::pin<quan::stm32::gpioa,13>  blue_led_pin;
typedef quan::mcu::pin<quan::stm32::gpioa,14>  green_led_pin;

/*
   ADC needs tim1/ tim2/tim3/tim4/tim5 or tim8
   available timers
*/

/*
   tim1  pan motor
   tim4  QDRT
   tim5  adc trigger
   tim6  best for DAC
   tim7  best for DAC
   tim8  
   tim10  frsky pwm
   tim11   tilt servo pwm
   tim13  // timer available but no output pins available onquantracker_osd
   tim14  // timer available but no output pins available onquantracker_osd
*/


/*
  tim4 16 bit timer
*/
typedef quan::stm32::tim4   azimuth_quadrature_counter; // for pan servo

typedef quan::mcu::pin<quan::stm32::gpiob,6> azimuth_encoder_ch1_pin;
typedef quan::mcu::pin<quan::stm32::gpiob,7> azimuth_encoder_ch2_pin;

typedef quan::stm32::tim5   adc_trigger_timer; 

/*
( for qdrt either lose a sp or a i2c )
 Currently choose to lose a SP so that I2C1 can be used  by LCD

(QDRT also takes over the timer)

       qdrtA                     PA0 TIM5_CH1
       qdrtb                     PA1 TIM5_CH2

Use TIM4 for pan motor timings also can trigger ADC

--- tilt servo
       tilt_servo_pwm            TIM11-CH1 PB9
--- sliprings_sp
       tx                        Mavlink port USART1 TX
       rx                        Mavlink Port USART1 Rx
*/
typedef quan::stm32::usart1                    gcs_serial_usart;
// fixed  on Arduino Serial connector
typedef quan::mcu::pin<quan::stm32::gpioa,9>   gcs_serial_txo_pin;
typedef quan::mcu::pin<quan::stm32::gpioa,10>  gcs_serial_rxi_pin;

// gcs_serial
typedef quan::stm32::freertos::usart_tx_rx_task<
   gcs_serial_usart,
   200,200,
   gcs_serial_txo_pin,gcs_serial_rxi_pin,
   char
> gcs_serial;

//----------------------------------------------------------------

typedef quan::stm32::uart4                   modem_usart;

typedef quan::mcu::pin<quan::stm32::gpioa,0>  modem_txo_pin;
typedef quan::mcu::pin<quan::stm32::gpioa,1>  modem_rxi_pin;

// modem
typedef quan::stm32::freertos::usart_tx_rx_task<
   modem_usart,
   200,200,
   modem_txo_pin,modem_rxi_pin,
   char
> modem_serial;

/*
--- frsky/mavlink telem
       tx                        USART4-TXO PA0
       rx                        USART4-RXI PA1
--- magnetometer (baro)
       scl                       PA8 I2C3-SCL
       sda                       PC9 I2C3-SDA
       rdy                       PA12  mag_rdy_exti_pin
*/
//typedef quan::mcu::pin<quan::stm32::gpioa,8> i2c3_scl;
//typedef quan::mcu::pin<quan::stm32::gpioc,9> i2c3_sda;
////typedef quan::stm32::i2c3  i2c_mag_port;
//typedef quan::stm32::freertos::freertos_i2c_task<
//   quan::stm32::i2c3,i2c3_scl,i2c3_sda
//> i2c_mag_port;
//
//typedef quan::mcu::pin<quan::stm32::gpioa,12> mag_rdy_exti_pin;

/*
--- MPU6000 spi
       MISO                      PB4 SPI1_MISO
       MOSI                      PB5 SPI1_MOSI
       SCK                       PB3 SPI1_SCK
--- GPS
       tx                        Mavlink Port  TX2
       rx                        Mavlink Port RX2
--- LCD

---  RC out
       pwm                       PA11 TIM1-CH4
--- battery level warning
      batt_level                 PB1  ADC12_IN9

--- serial LCD inteface
     tx                          PC12 USART5-TXO
*/


#endif // QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED
