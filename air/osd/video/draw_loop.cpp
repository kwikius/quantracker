#include "graphics_api.hpp"
#include "video.hpp"
#include "video_buffer.hpp"
#include <quan/dynarray.hpp>
#include "../resources.hpp"

void get_data_to_transmit(quan::dynarray<uint8_t> & ar);

namespace{
   bool error_flag  = false;
   void on_memory_error()
   {
      error_flag = true;
   }
}
void set_text_data( const char* text);
void draw_loop()
{
    static bool begin = true;
    typedef video_buffers::telem::rx rx;
    if (begin){
      begin = false;
         if ( is_receiver()){
            if ( is_transmitter()){
               set_text_data("transmitter/receiver - no data received yet");
            }else{
               set_text_data("receiver - no data received yet");
            }
         }else {
            if ( is_transmitter()){
               set_text_data("transmitter");
            }else{
               set_text_data("error - not transmitter or receiver");
            }
         }
    }else{
      if ( is_receiver()){
          while (!rx::manager.swapped()){;}
          // rx buffer now has received data
          uint8_t* rx_data = &rx::manager.m_read_buffer->front();
          char* cdata = ( char*) rx_data;
          cdata [video_buffers::telem::tx::get_num_data_bytes()-1] = '\0';
          set_text_data(cdata);
          rx::manager.request_swap();
      }
    }
   if ( error_flag){
      set_text_data("memory error");
   }else {
      if(on_draw()){
         // do other tasks...;
         typedef video_buffers::osd osd;
         typedef video_buffers::telem::tx tx;
         if ( is_transmitter()){
            quan::dynarray<uint8_t> data{tx::get_num_data_bytes(),on_memory_error};
            get_data_to_transmit(data);
            tx::write_data(data.get());
         }
         osd::manager.request_swap();
         if ( is_transmitter()){
            tx::manager.request_swap();
            while (! (osd::manager.swapped() && tx::manager.swapped())){;}
         }else{
             while (! osd::manager.swapped()){;}
         }
         osd::clear_write_buffer();
         if(is_transmitter()){
            tx::reset_write_buffer();
         }
      }
   }
}