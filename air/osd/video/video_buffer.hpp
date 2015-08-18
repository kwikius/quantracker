#ifndef OSD_VIDEO_BUFFER_HPP_INCLUDED
#define OSD_VIDEO_BUFFER_HPP_INCLUDED

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
 
#include <cstdint>
#include <cstring>
#include <utility>
#include <array>
#include <quan/two_d/vect.hpp>
#include <quan/stm32/bitband.hpp>
#include "osd_state.hpp"

template <uint32_t Length>
struct black_white_buffer_t {
   uint32_t * bb_black;
   uint8_t black [Length];
   uint32_t * bb_white;
   uint8_t white [Length];
   void init()
   {
      bb_black = quan::stm32::get_sram_bitband_address (black,0);
      bb_white = quan::stm32::get_sram_bitband_address (white,0);
   }
};

#if defined QUAN_OSD_TELEM_TRANSMITTER
// for tx
template <uint32_t Length>
struct white_buffer_t {
   uint32_t * bb_white;
   uint8_t white [Length];
   void init()
   {
      bb_white = quan::stm32::get_sram_bitband_address (white,0);
   }
};
#endif

struct video_params {
 
   struct osd {
      static constexpr uint8_t bits_pixel = 2;
      static constexpr uint8_t num_buffers = 2;
      struct buffer {
         //static constexpr uint32_t memory_size = 0x1F000;
         static constexpr uint32_t memory_size = 0xD000;
         static constexpr uint32_t length = (memory_size / (bits_pixel * num_buffers)) - 4 ;
         typedef black_white_buffer_t<length> type;
      };
      static constexpr uint32_t max_pixels = buffer::length * 8U;
   };
// n.b only need a tx or rx 
   struct telem {
#if defined QUAN_OSD_TELEM_TRANSMITTER
    //  static constexpr uint32_t memory_size = 0x1000;
      struct tx {
         struct buffer {
            static constexpr uint32_t memory_size = 0x400;
            static constexpr uint32_t length = (memory_size / 4U) - 4U ;
            typedef white_buffer_t<length> type;
         };
      };
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
      struct rx {
         struct buffer{
            static constexpr uint32_t memory_size = 0x400;
              // = telem::memory_size - telem::tx::buffer::memory_size;
            static constexpr uint32_t length = memory_size / 2;
            typedef std::array<uint8_t,length> type;
         };
      };
#endif
   };
};

template <typename Buffer> struct double_buffer_manager {
   typedef Buffer buffer_type;
   double_buffer_manager(buffer_type* r, buffer_type* w)
   :m_read_buffer{r},m_write_buffer{w},m_read_index{0},m_new_buffer_ready{false}{}

   void swap()
   {
      std::swap (m_read_buffer,m_write_buffer);
      m_new_buffer_ready = false;
   }
   
   void  read_reset()
   {
      m_read_index = 0U;
   }
   
   bool swapped()
   {
      return m_new_buffer_ready == false;
   }
   
   void request_swap()
   {
      m_new_buffer_ready = true;
   }
   
   void read_advance (uint32_t distance)
   {
      m_read_index += distance;
   }

//private:
   buffer_type* volatile  m_read_buffer;
   buffer_type* volatile  m_write_buffer;
   uint32_t m_read_index;
   volatile bool m_new_buffer_ready;
};
 
struct video_buffers {
   static void init();
   struct osd{
      static void clear_write_buffer()
      {
         uint32_t const active_buffer_len = (m_display_size.x/8 +1) * (m_display_size.y +1);
         if (osd_state::get() == osd_state::external_video){
            memset (manager.m_write_buffer->black,0xff,active_buffer_len);
            memset (manager.m_write_buffer->white,0xff,active_buffer_len);
         }else{
            memset (manager.m_write_buffer->black,0x0,active_buffer_len);
         }
      }

      static void clear_read_buffer()
      {
         uint32_t const active_buffer_len = (m_display_size.x/8 +1) * (m_display_size.y +1);
         if (osd_state::get() == osd_state::external_video){
            memset (manager.m_read_buffer->black,0xff,active_buffer_len);
            memset (manager.m_read_buffer->white,0xff,active_buffer_len);
         }else{
            memset (manager.m_write_buffer->black,0x0,active_buffer_len);
         }
      }

      static uint8_t * get_black_read_pos() 
      {
         return & manager.m_read_buffer->black[manager.m_read_index];
      }

      static uint8_t * get_white_read_pos() 
      {
         return & manager.m_read_buffer->white[manager.m_read_index];
      }

      static void set_buffer(uint32_t offset32, uint32_t mask, uint8_t val)
      {
// todo no_video case
         uint32_t * const black_ptr = ((uint32_t*)manager.m_write_buffer->black) + offset32;
         uint32_t * const white_ptr = ((uint32_t*)manager.m_write_buffer->white) + offset32;
#if 0
         switch c:
            case 0 : // gray switch addr 0
              *white_ptr  &= (0xffffffff & ~mask);
              *black_ptr  & =(0xffffffff & ~mask);
            break;
            case 1:   // white switch addr 1
              *white_ptr  |= mask ;
              *black_ptr  & =(0xffffffff & ~mask);
            break;
            case 2:
              *white_ptr  & =(0xffffffff & ~mask);
              *black_ptr  |= mask ;
            break;
            case 3:
               *black_ptr  |= mask ;
               *white_ptr  |= mask ;
            break;
         }
#else   
         if ( val & 0b10){
            *white_ptr  |= mask ;
         }else{
            *white_ptr  &= (0xffffffff & ~mask);
         }

         if ( val & 0b01){
            *black_ptr  |= mask ;
         }else{
            *black_ptr  &= (0xffffffff & ~mask);
         }
#endif
      }
      
      static void xy_to_buf (quan::two_d::vect<int32_t> const & px,uint8_t val)
      {
         if ( (px.x < 0)
               || (px.y < 0)
               || (static_cast<uint32_t> (px.y) > (m_display_size.y-1) )
               || (static_cast<uint32_t> (px.x) > (m_display_size.x-1))
            )
         {
            return;
         }
         
         uint32_t const buffer_bit_pos
         = static_cast<uint32_t> (px.y) * (m_display_size.x + 8)
           + static_cast<uint32_t> (px.x) + 1U;
           
         
         manager.m_write_buffer->bb_black[buffer_bit_pos] = val;
         // though white not used in external video mode
         // just ignore that so external mode isnt slowed
         manager.m_write_buffer->bb_white[buffer_bit_pos] = val >> 1 ;

      }

      static uint8_t get_colour(quan::two_d::vect<int32_t> const & px )
      {
         if ( (px.x < 0)
               || (px.y < 0)
               || (static_cast<uint32_t> (px.y) > (m_display_size.y-1) )
               || (static_cast<uint32_t> (px.x) > (m_display_size.x-1))
            )
         {
            return ((osd_state::get() == osd_state::external_video)? 3 : 2); //transparent: black
         }
         uint32_t const buffer_bit_pos
         = static_cast<uint32_t> (px.y) * (m_display_size.x + 8)
           + static_cast<uint32_t> (px.x) + 1U;
         uint8_t colour = ((manager.m_write_buffer->bb_black[buffer_bit_pos] !=0 )?2:0);
         // this will fail then unless
         if (osd_state::get() == osd_state::external_video){
            colour |= ((manager.m_write_buffer->bb_white[buffer_bit_pos]!=0)?1:0);
         }
         return colour;
      }

      static double_buffer_manager<video_params::osd::buffer::type> manager;
      static quan::two_d::vect<uint32_t> m_display_size; //pixels
      static video_params::osd::buffer::type m_buffers[2];
   };

   struct telem{
#if defined QUAN_OSD_TELEM_TRANSMITTER || defined QUAN_OSD_TELEM_RECEIVER
      struct tx{
         static  constexpr uint32_t sol_bits = 4U;
         static  constexpr uint32_t eol_bits = 4U;
         static uint32_t get_full_bytes_per_line()
         {
            return (m_size.x / 8U) + ((m_size.x % 8U) ? 1 : 0);
         }
#if defined QUAN_OSD_TELEM_TRANSMITTER
         static void reset_write_buffer()
         {
//
            uint32_t const active_buffer_len = get_full_bytes_per_line() * m_size.y;
            // set to white == mark state
          //  memset (manager.m_write_buffer->black,0xff,active_buffer_len);
          //  if (osd_state::get() == osd_state::external_video){
               memset (manager.m_write_buffer->white,0xff,active_buffer_len);
//            }else{
//               memset (manager.m_write_buffer->white,0,active_buffer_len);
//            }
         }
         
         // call reset_write_buffer first
         // for data 1 is space 0 is mark
         // means that 0 data will be at black level
         static void write_data ( uint8_t * ar);
         
         static void reset_read_buffer()
         {
            uint32_t const active_buffer_len = get_full_bytes_per_line() * m_size.y;
            memset (manager.m_write_buffer->white,0xff,active_buffer_len);
         }

         static uint8_t * get_white_read_pos() 
         {
            return & manager.m_read_buffer->white[manager.m_read_index];
         }
         
         static double_buffer_manager<video_params::telem::tx::buffer::type> manager;
         static video_params::telem::tx::buffer::type m_buffers[2];
#endif
         static quan::two_d::vect<uint32_t> m_size; 
 
         static uint32_t get_data_bytes_per_line ()
         {
            return (m_size.x - (sol_bits + eol_bits)) / 10U ;
         }
         static uint32_t get_num_lines()
         {
             return m_size.y;
         }
         static uint32_t get_num_data_bytes()
         {
            return get_data_bytes_per_line() * get_num_lines();
         }
      };
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
      struct rx{

         static double_buffer_manager<video_params::telem::rx::buffer::type> manager;
         static video_params::telem::rx::buffer::type m_buffers[2];
          
         static uint32_t get_num_data_bytes()
         {
            return tx::get_num_data_bytes();
         }
         static void reset_read_buffer()
         {
             memset(&manager.m_read_buffer[0],0,get_num_data_bytes());
         }
         static void reset_write_buffer()
         {
            memset(&manager.m_write_buffer[0],0,get_num_data_bytes());
         }
         
      };
#endif
   };

};
 
#endif // OSD_VIDEO_BUFFER_HPP_INCLUDED
 
 