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
#if !(defined QUAN_OSD_TELEM_TRANSMITTER)
#error only for use with telem transmitter
#endif

#include <cstdio>
#include <cstring>

#include <stm32f4xx.h>

#include "FreeRTOS.h"
#include <task.h>

//#include <quan/dynarray.hpp>
#include <quan/time.hpp>
#include <quan/frequency.hpp>

#include "resources.hpp"
#include "../../../air/osd/video/video_cfg.hpp"
#include "../../../air/osd/video/video_buffer.hpp"

/*
   get the data to transmit into ar provided by the caller
   The function reallocates the dynarray to the (constant) size of telem data for one half frame) 
   NB. Need to sort some way to know what is valid data.
   Use COBS to encode the data in one packet. After the packet just put zeroes.
   Alternatively, if its the same data then its length should be known
   May need a header
*/
namespace detail{
   void create_telem_tx_swap_semaphores();
   void swap_telem_tx_buffers();
}

namespace {

   uint8_t* telem_tx_buffer = nullptr;
   bool init_telem_tx_buffer()
   {
      telem_tx_buffer = (uint8_t*) pvPortMalloc(video_buffers::telem::tx::get_num_data_bytes());
      return telem_tx_buffer != nullptr;
   }

// shouldnt be here . 
//Should be external so definable by user
// might be better to do a get read_new_telem_tx data_from(buffer)
   //make global and rename to on_telemetry_transmitted
   // add functios
   // to get length of tx telem buffer
   // and to write to the buffer
   void put_telemetry_tx_data()
   {
       int64_t time_now = xTaskGetTickCount() * (1000 / configTICK_RATE_HZ); //( ms)
       int min_now = static_cast<int>(time_now / 60000);
       int s_now   = static_cast<int>((time_now / 1000) - (min_now * 60));
       sprintf((char*)telem_tx_buffer,"time = %03d min %02d s", min_now,s_now);
             // fill the rest with zeroes
       uint32_t const data_size = video_buffers::telem::tx::get_num_data_bytes();
       auto len = strlen ((char*)telem_tx_buffer) +1;
       memset (telem_tx_buffer + len ,0,data_size -  len  );
       video_buffers::telem::tx::write_data(telem_tx_buffer);
   }

   void telem_tx_task(void* params)
   {
  
      if ( init_telem_tx_buffer() == true ){
         for(;;){
            put_telemetry_tx_data();
            detail::swap_telem_tx_buffers();
         }
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;

}//namespace

void create_vsync_telem_tx_task()
{
   detail::create_telem_tx_swap_semaphores();
   xTaskCreate(
      telem_tx_task,"telem_tx_task", 
      600,
      &dummy_param,
      task_priority::vsync_telem_tx,
      &task_handle
   );
}

void vsync_telem_tx_task_setup()
{
//todo redo transmitter using usart
// Shutdown TLV3501 output
#if (QUAN_OSD_BOARD_TYPE == 4) 
   quan::stm32::module_enable<telem_cmp_enable_pin::port_type>();
   quan::stm32::apply<
      telem_cmp_enable_pin   
      ,quan::stm32::gpio::mode::input 
      ,quan::stm32::gpio::pupd::pull_up
   >();
   quan::stm32::module_enable<av_telem_rx::port_type>();
   quan::stm32::apply<
      av_telem_rx   
      ,quan::stm32::gpio::mode::input //cmp output hiz pulled up
      ,quan::stm32::gpio::pupd::pull_up
   >();
#endif
}

