#include <cstdio>
#include <cstring>

#include <stm32f4xx.h>

#include "FreeRTOS.h"
#include <task.h>

#include <quan/dynarray.hpp>
#include <quan/time.hpp>
#include <quan/frequency.hpp>

#include "../resources.hpp"
#include "video_cfg.hpp"
#include "video_buffer.hpp"

/*
   get the data to transmit into ar provided by the caller
   The function reallocates the dynarray to the (constant) size of telem data for one half frame) 
   NB. Need to sort some way to know what is valid data.
   Use COBS to encode the data in one packet. After the packet just put zeroes.
   Alternatively, if its the same data then its length should be known
   May need a header
*/

void create_telem_swap_semaphores();
void swap_telem_buffers();

namespace {

   bool error_flag  = false;
   void on_memory_error()
   {
      error_flag = true;
   }

   void do_time (quan::dynarray<uint8_t>  & ar)
   {
       int64_t time_now = xTaskGetTickCount() * (1000 / configTICK_RATE_HZ); //( ms)
       int32_t min_now = static_cast<int32_t>(time_now / 60000);
       int32_t s_now   = (time_now / 1000) - (min_now * 60);
       sprintf((char*)ar.get(),"time = %03lu min %02lu s", min_now,s_now);
   }

   void get_data_to_transmit (quan::dynarray<uint8_t> & ar)
   {
      // data_size is the constant total bytes that are transmitted in
      // one half frame ( ie between 2 vsyncs)
      uint32_t const data_size = video_buffers::telem::tx::get_num_data_bytes();
      ar.realloc (data_size);
      do_time(ar);
      // fill the rest with zeroes
      auto len = strlen ((char*)ar.get()) +1;
      memset (ar.get() + len ,0,data_size -  len  );
   }

   void telem_task(void* params)
   {
      create_telem_swap_semaphores();

      for(;;){
         typedef video_buffers::telem::tx tx;
         quan::dynarray<uint8_t> data{tx::get_num_data_bytes(),on_memory_error};
         get_data_to_transmit(data);
         tx::write_data(data.get());
         swap_telem_buffers();
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;

}//namespace

void create_telem_task()
{
   xTaskCreate(
      telem_task,"telem_task", 
      500,
      &dummy_param,
      task_priority::av_telemetry,
      &task_handle
   );
}

namespace {
 
   void av_telem_usart_setup()
   {
//todo redo transmitter using usart
#if defined QUAN_OSD_TELEM_RECEIVER
      quan::stm32::module_enable<telem_cmp_enable_pin::port_type>();
      quan::stm32::apply<
         telem_cmp_enable_pin    // TIM2_CH2 or TIM2_CH4 for boardtype 4 (PA3)
         ,quan::stm32::gpio::mode::af1 // same for all boardtypes 
         ,quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::module_enable<av_video_rxi::port_type>();
      quan::stm32::apply<
         av_video_rxi
#if (QUAN_OSD_BOARD_TYPE == 4)
         ,quan::stm32::gpio::mode::af8 // PC7  USART6_RX
#else
         ,quan::stm32::gpio::mode::af7
#endif
         ,quan::stm32::gpio::pupd::pull_up
      >();
      
      quan::stm32::module_reset<av_telemetry_usart>();
      quan::stm32::module_enable<av_telemetry_usart>();
      
      quan::stm32::apply<
         av_telemetry_usart
         ,quan::stm32::usart::asynchronous
         ,quan::stm32::usart::transmitter<false>
         ,quan::stm32::usart::receiver<true>
         ,quan::stm32::usart::baud_rate<2000000,false>
         ,quan::stm32::usart::parity::none
         ,quan::stm32::usart::data_bits<8>
         ,quan::stm32::usart::stop_bits<1>
         ,quan::stm32::usart::rts<false>
         ,quan::stm32::usart::cts<false>
         ,quan::stm32::usart::i_en::cts<false>
         ,quan::stm32::usart::i_en::lbd<false>
         ,quan::stm32::usart::i_en::txe<false>
         ,quan::stm32::usart::i_en::tc<false>
         ,quan::stm32::usart::i_en::rxne<false>
         ,quan::stm32::usart::i_en::idle<false>
         ,quan::stm32::usart::i_en::pe<false>
         ,quan::stm32::usart::i_en::error<false>
      >();
     // av_telemetry_usart::get()->cr3.setbit<6>(); //( DMAR)
      av_telemetry_usart::get()->cr1.setbit<2>(); // ( RE)
#else  // transmitter
  #if (QUAN_OSD_BOARD_TYPE == 4) 
      quan::stm32::module_enable<telem_cmp_enable_pin::port_type>();
      quan::stm32::apply<
         telem_cmp_enable_pin   
         ,quan::stm32::gpio::mode::input // Shutdown TLV3501 kep cmp disabled
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
}//namespace 

void av_telem_dma_setup();
 
void av_telem_setup()
{
  // av_telem_usart_setup();
  // av_telem_dma_setup();
}
 