#ifndef QUANTRACKER_SERIAL_PORTS_HPP_INCLUDED
#define QUANTRACKER_SERIAL_PORTS_HPP_INCLUDED

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

#include <quan/stm32/serial_port.hpp>
#include <quan/stm32/rx_serial_port.hpp>
#include <quan/stm32/tx_serial_port.hpp>
#include "resources.hpp"

struct sliprings{
   typedef rctx_txo_pin txo_pin;
   typedef rctx_rxi_pin rxi_pin;
   static constexpr uint32_t in_buf_size = 1000;
#ifdef DEBUG
   static constexpr uint32_t out_buf_size = 1000;
#else
  static constexpr uint32_t out_buf_size = 25;
#endif
   typedef quan::stm32::serial_port<
   sliprings_usart,out_buf_size,in_buf_size,txo_pin,rxi_pin
   > serial_port;
};

struct airosd{
   typedef air_osd_tx_out_pin txo_pin;
   typedef air_osd_rx_in_pin rxi_pin;
   static constexpr uint32_t in_buf_size = 100;
   static constexpr uint32_t out_buf_size = 100;
   typedef quan::stm32::serial_port<
   air_osd_serial_port,out_buf_size,in_buf_size,txo_pin,rxi_pin
   > serial_port;
};

struct av_fsk{

   typedef av_telem_rx_pin rxi_pin;
   static constexpr uint32_t in_buf_size = 100;
   typedef quan::stm32::rx_serial_port<
      av_telem_uart,in_buf_size,rxi_pin
   > serial_port;
};

typedef sliprings debug;

#endif // QUANTRACKER_SERIAL_PORTS_HPP_INCLUDED
