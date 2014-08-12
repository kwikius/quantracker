#ifndef FSK_TX_FSK_PARAMS_HPP_INCLUDED
#define FSK_TX_FSK_PARAMS_HPP_INCLUDED
/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */


#include <quan/stm32/detail/get_bus.hpp>
#include <quan/stm32/sys_freq.hpp>
#include <quan/meta/integer_max.hpp>
#include <quan/stm32/tim.hpp>
#include <quan/meta/gcd.hpp>

struct fsk_params{
 #if 1
   // BELL 202
   static constexpr uint32_t F_high = 2200U ; //Hz
   static constexpr uint32_t F_low = 1200U; // Hz
   static constexpr uint32_t mux = 2;
#else
   // CCIT V.23 mode 2
   static constexpr uint32_t F_high = 2100U ; //Hz
   static constexpr uint32_t F_low = 1300U; // Hz
   static constexpr uint32_t mux = 4;
#endif
   //???
   static constexpr uint32_t dac_write_freq = mux * (F_high * F_low)/quan::meta::gcd<uint32_t,F_high,F_low>::value; // Hz
   static constexpr uint32_t raw_low_freq_steps = dac_write_freq / F_high; // dimless
   static constexpr uint32_t raw_high_freq_steps = dac_write_freq / F_low; // dimless
   static constexpr uint32_t steps_gcd = quan::meta::gcd<uint32_t,raw_low_freq_steps,raw_high_freq_steps>::value;
   static constexpr uint32_t low_freq_steps = raw_low_freq_steps/steps_gcd; // dimless
   static constexpr uint32_t high_freq_steps = raw_high_freq_steps/steps_gcd; // dimless
   static constexpr uint32_t num_sin_values = (raw_low_freq_steps * raw_high_freq_steps)/steps_gcd;
   static constexpr uint32_t baud_rate = 1200U;
   static constexpr uint32_t samples_per_bit = dac_write_freq / baud_rate;
   static_assert(dac_write_freq % baud_rate == 0, "remainder in division");
   // want to no bus_speed of tim6 bus
   static constexpr uint32_t tim6_bus_freq = quan::stm32::get_bus_frequency<quan::stm32::detail::get_bus<quan::stm32::tim6>::type>();
#if defined QUAN_STM32F0
   static_assert(tim6_bus_freq == 48000000,"not as expected");
#endif
  // this basicallys sets modem baudrate
   static constexpr uint32_t clks_per_dac_write = tim6_bus_freq/dac_write_freq + (((tim6_bus_freq % dac_write_freq) >= dac_write_freq/2)?1:0);
// ##########Wrong currently #########
 //  uint32_t prescale_value = (clks_per_dac_write  & 0xFFFF0000) >> 16;
   static_assert ( clks_per_dac_write <= quan::meta::integer_max<uint16_t>::value, "require prescaler");

   typedef  uint16_t dac_data_type;
   static constexpr uint16_t dac_max_value = 4095;// quan::meta::integer_max<dac_data_type>::value;
   static constexpr double dac_pp_voltage = 1.5;  // required amplitude in volts
   static constexpr double supply_voltage = 3.3;
};

#endif // FSK_TX_FSK_PARAMS_HPP_INCLUDED
