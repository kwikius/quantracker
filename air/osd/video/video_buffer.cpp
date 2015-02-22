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

#include "video_cfg.hpp"
#include "video_buffer.hpp"

video_params::osd::buffer::type 
video_buffers::osd::m_buffers[2] __attribute__((section(".display_buffer")));

video_params::telem::tx::buffer::type 
video_buffers::telem::tx::m_buffers[2] __attribute__((section(".telem_buffer")));

video_params::telem::rx::buffer::type 
video_buffers::telem::rx::m_buffers[2] __attribute__((section(".telem_buffer")));

double_buffer_manager<video_params::osd::buffer::type>
video_buffers::osd::manager{
   &video_buffers::osd::m_buffers[0],
   &video_buffers::osd::m_buffers[1]
};

double_buffer_manager<video_params::telem::tx::buffer::type>
video_buffers::telem::tx::manager{
   &video_buffers::telem::tx::m_buffers[0],
   &video_buffers::telem::tx::m_buffers[1]
};

double_buffer_manager<video_params::telem::rx::buffer::type>
video_buffers::telem::rx::manager{
   &video_buffers::telem::rx::m_buffers[0],
   &video_buffers::telem::rx::m_buffers[1]
};

quan::two_d::vect<uint32_t> video_buffers::osd::m_display_size; //pixels

quan::two_d::vect<uint32_t> video_buffers::telem::tx::m_size; //pixels

//bool video_buffers::telem::tx::m_want_tx = false;

void video_buffers::init()
{
    osd::m_buffers[0].init();
    osd::m_buffers[1].init();
    video_buffers::osd::m_display_size = video_cfg::get_display_size_px();
    osd::clear_read_buffer();
    osd::clear_write_buffer();
    osd::manager.read_reset();
    telem::tx::m_size 
      = quan::two_d::vect<uint32_t>{
         video_cfg::columns::telem::get_count()
         ,video_cfg::rows::telem::get_count()
        };
    telem::tx::m_buffers[0].init();
    telem::tx::m_buffers[1].init();
    telem::tx::reset_read_buffer();
    telem::tx::reset_write_buffer(); 
    telem::tx::manager.read_reset();
}
// call reset_write_buffer first
// for data 1 is space 0 is mark
// means that 0 data will be at black level
void video_buffers::telem::tx::write_data ( uint8_t * ar)
{
   for ( uint32_t y = 0 ,yend = get_num_lines(); y < yend; ++y){ // rows
     // start of line mark state == transparent
     uint32_t bit_offset = y * 8 * get_full_bytes_per_line() + sol_bits;
     for ( uint32_t xbyte = 0, xend = get_data_bytes_per_line(); xbyte < xend; ++xbyte){ // columns
         // start bit
         manager.m_write_buffer->bb_white[bit_offset] = 0b0;
         ++bit_offset;
         uint8_t const cur_val = *ar; 
         for ( uint32_t bitpos = 0U; bitpos < 8U; ++bitpos){
            if( (cur_val & ( 1U << bitpos)) == 0U) {  
               manager.m_write_buffer->bb_white[bit_offset] = 0b0;
            }
            ++bit_offset;
         }
         // stop bit
         ++bit_offset;
         ++ar;
      }
      // rest of line transparent == mark state
   }
}



