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

void create_telem_tx_swap_semaphores();
void swap_telem_tx_buffers();
//void put_telemetry_tx_data();

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
   void put_telemetry_tx_data()
   {
       int64_t time_now = xTaskGetTickCount() * (1000 / configTICK_RATE_HZ); //( ms)
       int32_t min_now = static_cast<int32_t>(time_now / 60000);
       int32_t s_now   = (time_now / 1000) - (min_now * 60);
       sprintf((char*)telem_tx_buffer,"time = %03lu min %02lu s", min_now,s_now);
             // fill the rest with zeroes
       uint32_t const data_size = video_buffers::telem::tx::get_num_data_bytes();
       auto len = strlen ((char*)telem_tx_buffer) +1;
       memset (telem_tx_buffer + len ,0,data_size -  len  );
       video_buffers::telem::tx::write_data(telem_tx_buffer);
   }

   void telem_tx_task(void* params)
   {
      create_telem_tx_swap_semaphores();
      if ( init_telem_tx_buffer() == true ){
         for(;;){
            put_telemetry_tx_data();
            swap_telem_tx_buffers();
         }
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;

}//namespace

void create_telem_tx_task()
{
   xTaskCreate(
      telem_tx_task,"telem_task", 
      600,
      &dummy_param,
      task_priority::av_telemetry_tx,
      &task_handle
   );
}

void av_telem_tx_setup()
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

