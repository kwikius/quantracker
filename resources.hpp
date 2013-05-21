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

#include <quan/stm32f4/usart.hpp>
#include <quan/stm32f4/gpio.hpp>
#include <quan/stm32f4/tim.hpp>

typedef quan::stm32f4::tim2 main_loop_timer;
typedef quan::stm32f4::tim3 azimuth_qdrt_counter;

 /*
  using loop_timer  (tim2)
  loop_timer.CC1 used for azimuth motor PWM
  loop_timer CC2 used for elevation servo pwm
  loop_timer.CC3 used for FrSky RcTx Pulses
  loop_timer.CC4 used to gen interrupt to do calc b4 looptimer overrflow
*/

typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,0>    user_switch_pin;  
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,1>    elev_servo_pwm_out_pin;                // SF: TIM2_CH2:AF1
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,2>    rc_tx_out_pin;                         // SF: TIM2_CH3:AF1
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,3>    switch_set_zero_pin;   
// PA4 DAC out x?
// PA5 DAC out xx?                   

typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,8>    not_azimuth_motor_direction_out_pin;   //( H-bridge blue wire in1, in4)
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioa,15>   azimuth_motor_pwm_out_pin;             // SF: TIM2_CH1:AF1

typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,0>    azimuth_motor_direction_out_pin;       // (H-bridge white in2,in3)
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,1>    switch_set_home_pin;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,2>    switch_tracking_pin;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,4>    button_left_pin;
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,5>    azimuth_encoder_a_pin;                 // SF: TIM3_CH2:AF2  ( encoder yellow 6th from left) 
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,6>    eeprom_scl;              
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,7>    button_right_pin;
// pb8 free
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiob,9>    eeprom_sda;
// pb11 free
// pb12 free
// pb13 free
// pb14 free
// pb15 free

// pc1 free
// pc2 free
// pc4 free
// pc5 free
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpioc,6>    azimuth_encoder_b_pin;                 // SF: TIM3_CH1:AF2 ( encoder white, 5th from left)
// pc8 free
// pc9 free
// pc11 free

typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,5>    frsky_txo_pin;                         // SF:USART2_TX:AF7(!!!remove R50 on Discovery!!!)
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,6>    frsky_rxi_pin;                         // SF:USART2_RX:AF7
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,8>    rctx_txo_pin;                          // SF:USART3_TX:AF7
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,9>    rctx_rxi_pin;                          // SF:UASRT3_RX:AF7

typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,12>   green_led_pin;   // green led on Discovery
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,13>   orange_led_pin;       // orange led on Discovery
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,14>   red_led_pin;  // red led on Discover
typedef quan::stm32f4::gpio::pin<quan::stm32f4::gpiod,15>   blue_led_pin;       // blue led on Discover

typedef quan::stm32f4::usart3 rctx_usart;
typedef quan::stm32f4::usart2 frsky_usart;

struct interrupt_priority{
   static constexpr uint32_t systick_timer = 15;
   static constexpr uint32_t frsky_serial_port= 14;
   static constexpr uint32_t rctx_serial_port = 13;
   static constexpr uint32_t loop_timer = 12;
};

#endif // QUANTRACKER_RESOURCES_HPP_INCLUDED
