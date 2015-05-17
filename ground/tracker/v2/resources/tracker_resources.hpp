#ifndef QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED

#include <quan/voltage.hpp>
#include <quan/stm32/gpio.hpp>
#include <quan/stm32/spi.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/stm32/usart.hpp>

#include <quan/stm32/freertos/freertos_usart_task.hpp>

/*
non OSD resources used by the tracker
also add some leds
push buttons etc

--- pan motor
       motor_direction           PC14
       motor pwm                 PB8  

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
       rdy                       PC5
--- MPU6000 spi
       MISO                      PB4 SPI1_MISO
       MOSI                      PB5 SPI1_MOSI
       SCK                       PB3 SPI1_SCK
--- GPS
       tx                        Mavlink Port  TX2
       rx                        Mavlink Port RX2
--- fsk
       audioL  --- analog in     PC0  ADCXX
       audioR  --- analog in     PC1  ADCXX
--- LCD
       
---  RC out
       pwm                       PA11 TIM1-CH4   
--- battery level warning
      batt_level                 PB1  ADC12_IN9

--- serial LCD inteface
     tx                          PC12 USART5-TXO  
*/


#endif // QUANTRACKER_GROUND_TRACKER_V2_TRACKER_RESOURCES_HPP_INCLUDED
