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

#include <quan/malloc_free.hpp>
#include <quan/min.hpp>

#include <quantracker/osd/telemetry_transmitter.hpp>
#include "../../../air/osd/video/video_cfg.hpp"
#include "../../../air/osd/video/video_buffer.hpp"
#include "resources.hpp"
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

size_t get_telemetry_transmitter_buffer_length()
{
   return video_buffers::telem::tx::get_num_data_bytes();
}

namespace {
   // use this intermediate buffer to temporarily store users data
   // in case user has a short buffer etc

   uint8_t* telem_tx_buffer      = nullptr;
   size_t   telem_tx_buffer_size = 0;

   bool init_telem_tx_buffer(uint32_t size)
   {
      if(telem_tx_buffer_size == size){
         return true;
      }else{
         if( telem_tx_buffer != nullptr){
            quan::free(telem_tx_buffer);
         }
         telem_tx_buffer = (uint8_t*) quan::malloc(size);
         telem_tx_buffer_size = size;
         return telem_tx_buffer != nullptr;
      }
   }
}

/*
   The data written to the internal buffer
   is capped at the max buffer length
*/
size_t write_telemetry_data(const char * buffer, size_t len)
{
   if ( (buffer != nullptr) && (len > 0)){
      size_t const max_len = get_telemetry_transmitter_buffer_length();
      init_telem_tx_buffer(max_len);
      size_t const capped_len = quan::min(len,max_len);
      memcpy(telem_tx_buffer,buffer,capped_len);
      if ( capped_len < max_len){
         memset(telem_tx_buffer + capped_len ,0,max_len - capped_len  );
      }
      video_buffers::telem::tx::write_data(telem_tx_buffer);
      return capped_len;
   }else{
      return size_t{0};
   }
}

namespace {

   void telemetry_transmitter_task(void* params)
   {
      for(;;){
         // the user defined callback
         on_telemetry_transmitted();
         detail::swap_telem_tx_buffers();
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;

}//namespace

// create_telemetry_transmitter_task
void create_telemetry_transmitter_task()
{
   detail::create_telem_tx_swap_semaphores();
   xTaskCreate(
      telemetry_transmitter_task,"telemetry_transmitter_task", 
      600,
      &dummy_param,
      task_priority::vsync_telem_tx,
      &task_handle
   );
}

// setup_telemetry_transmitter_task
void setup_telemetry_transmitter_task()
{
//todo redo transmitter using usart
// Shutdown TLV3501 output
#if (QUAN_OSD_BOARD_TYPE == 4) 
   #if ! defined QUAN_AERFLITE_BOARD
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
#endif
}

