#ifndef FSK_TX_RESOURCES_HPP_INCLUDED
#define FSK_TX_RESOURCES_HPP_INCLUDED

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

// same on F4 and F0
typedef quan::mcu::pin<quan::stm32::gpioa,4>    dac_out_pin; 

typedef quan::mcu::pin<quan::stm32::gpioa,9>    posdata_txo_pin; 
typedef quan::mcu::pin<quan::stm32::gpioa,10>   posdata_rxi_pin; 

typedef quan::mcu::pin<quan::stm32::gpioa,2>    frsky_txo_pin;
typedef quan::mcu::pin<quan::stm32::gpioa,3>    frsky_rxi_pin;
 
#if defined QUAN_DISCOVERY
typedef quan::mcu::pin<quan::stm32::gpioc,8> heartbeat_led_pin; // stm32F0 Discovery blue led
#else
typedef quan::mcu::pin<quan::stm32::gpioa,0>  heartbeat_led_pin; 
#endif

typedef quan::stm32::usart1  posdata_usart;
typedef quan::stm32::usart2  frsky_usart;

//struct interrupt_priority{
//   static constexpr uint32_t systick_timer = 15;
//   static constexpr uint32_t frsky_serial_port= 14;
//   static constexpr uint32_t sliprings_serial_port = 13;
//   static constexpr uint32_t loop_timer = 12;
//};

#endif // FSK_TX_RESOURCES_HPP_INCLUDED
