#ifndef QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED

#include <quan/voltage.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>
//#include <quan/stm32/i2c_port.hpp>
#include <quan/stm32/freertos/freertos_i2c_task.hpp>
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
typedef quan::stm32::tim10                      video_level_dac_irq_timer;
typedef quan::stm32::tim12                      sync_sep_timer;
*/
typedef quan::mcu::pin<quan::stm32::gpiob,8>   pan_motor_pwm_pin;
typedef quan::mcu::pin<quan::stm32::gpioc,14>  pan_motor_dir_pin;

typedef quan::mcu::pin<quan::stm32::gpioc,0>   pan_motor_emf_adc_pin;
typedef quan::mcu::pin<quan::stm32::gpioc,1>   pan_motor_current_adc_pin;

/*
   ADC needs tim1/ tim2/tim3/tim4/tim5 or tim8
   available timers
   tim1   rc pwm 16 bit 4 channels - ADC trigger
*/

/*
   tim4  - ADC trigger
   tim5 ** used by QDRT 
   tim6  best for DAC
   tim7  best for DAC
   tim8    // 16 bit 4 channels   - ADC trigger
   tim11   tilt servo pwm
   tim13
   tim14
*/
 // 16 bit single channle timer
 // set up for 1 us clock
 typedef quan::stm32::tim13  pan_motor_timer ;
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
 typedef quan::stm32::usart1                    sliprings_usart;
 typedef quan::mcu::pin<quan::stm32::gpioa,9>   sliprings_txo_pin;
 typedef quan::mcu::pin<quan::stm32::gpioa,10>  sliprings_rxi_pin;

typedef quan::stm32::freertos::usart_tx_rx_task<
   sliprings_usart,
   100,100, 
   sliprings_txo_pin,sliprings_rxi_pin,
   char
> sliprings_tx_rx_task;

/*
--- frsky/mavlink telem 
       tx                        USART4-TXO PA0
       rx                        USART4-RXI PA1
--- magnetometer (baro)
       scl                       PA8 I2C3-SCL 
       sda                       PC9 I2C3-SDA 
       rdy                       PA12  mag_rdy_exti_pin
*/
  typedef quan::mcu::pin<quan::stm32::gpioa,8> i2c3_scl;
  typedef quan::mcu::pin<quan::stm32::gpioc,9> i2c3_sda;
  //typedef quan::stm32::i2c3  i2c_mag_port;
  typedef quan::stm32::freertos::freertos_i2c_task<
      quan::stm32::i2c3,i2c3_scl,i2c3_sda
  > i2c_mag_port;
  typedef quan::mcu::pin<quan::stm32::gpioa,12> mag_rdy_exti_pin;

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
