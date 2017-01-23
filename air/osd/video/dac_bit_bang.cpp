/*
 Copyright (c) 2013 -2015 Andy Little 

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

#if !defined QUAN_AERFLITE_BOARD

#include <stm32f4xx.h>
#include <quan/bit.hpp>
#include <quan/time.hpp>

#include <quan/frequency.hpp>
#include <quan/utility/fifo.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/constrain.hpp>
#include "osd_state.hpp"
#include "../resources.hpp"

#if (QUAN_OSD_BOARD_TYPE != 1 ) 

/*
 External DAC using DAC08S084S05 8 bit DAC. Uses bitbang spi to avoid use of SPI hardware
*/
namespace {

   // may be longer!
   void delay_10ns(uint16_t n)
   {
      for ( uint32_t i = 0; i < (2 * n ); ++i){
         asm volatile("nop":::);
      }
   }

   using quan::stm32::set;
   using quan::stm32::clear;
   using quan::stm32::put;

   void ll_dac_write(uint16_t val)
   {
      vTaskSuspendAll();
      uint16_t dac_data = val;
      clear<av_dac_nsync>();
      for ( uint8_t i = 0; i < 16; ++i){
         delay_10ns(2);  // min 10 ns
         put<av_dac_data>( (dac_data & (1U << (15U - i))) != 0U) ;
         delay_10ns(2);  // min 3.5 ns
         clear<av_dac_clk>();  // data is clocked on falling edges of clock
         delay_10ns(4);
         set<av_dac_clk>();
      }
      delay_10ns(2);
      set<av_dac_nsync>();
      xTaskResumeAll();
   }

}//namespace

namespace detail{
   bool dac_busy()
   {
      return false;
   } 
}

namespace {

   constexpr quan::voltage::V vref{3.28f};
   constexpr quan::voltage::V ze{0.005f};
   constexpr float fse = 0.0075f;
}

// shift right 4 for 8 bit dac
void Dac_write( uint8_t dacnum, quan::voltage::V const & vout, uint8_t code)
{
/*
  vout = val / 256 * vref * (1- fse) + ze
  vout - ze = val / 256 * vref * (1- fse)
  val = (Vout - ze) * 256  / (vref * (1 - fse))
*/
     quan::voltage::V const vout1 = quan::constrain(vout,quan::voltage::V{0},vref - ze);
     uint16_t const val 
      = static_cast<uint16_t>(((vout1 - ze) * (256.f) / (vref * (1.f - fse))) + 0.5f) << 4;
     uint16_t const data
     =   (static_cast<uint16_t>(val) & 0xFFF )
       | ((static_cast<uint16_t>(dacnum) & 0b11) << 14)
       | ((static_cast<uint16_t>(code) & 0b11 ) << 12);

    ll_dac_write(data);
}
namespace {

   void external_video_mode_dac_setup()
   {
      constexpr uint8_t dac_data_idx = 0; // (also grey)
      constexpr uint8_t dac_white_idx = 1;
      constexpr uint8_t dac_black_idx = 2;
      constexpr uint8_t dac_sync_idx = 3;
      //##########################
      Dac_write (dac_sync_idx, quan::voltage::V{0.58f}, 0);
      Dac_write (dac_black_idx, quan::voltage::V{0.90f}, 0); 
      Dac_write (dac_white_idx, quan::voltage::V{2.26f}, 0); 
      Dac_write (dac_data_idx, quan::voltage::V{1.58f}, 1);
   }

   void internal_video_mode_dac_setup()
   {
// in internal video mode
// only black and white available
// 00 (0) at dac is black_level
// 01 (1) at dac is white
// 10 (20 at dac is sync_tip
     // do need sync_comp may be best to write it now
       Dac_write (0b00, quan::voltage::V{0.90f}, 0); // black level
       Dac_write (0b01, quan::voltage::V{2.26f}, 0); // white
       Dac_write (0b10, quan::voltage::V{0.28f}, 0); // synctip
       Dac_write (0b11, quan::voltage::V{0.58f}, 1); // sync comp
   }

   void set_init_dac_values()
   {
      if( osd_state::get() == osd_state::external_video){
       external_video_mode_dac_setup();  
      }else{
         internal_video_mode_dac_setup();
      }
     // while (detail::dac_busy() ){asm volatile ("nop":::);}
   }
}

namespace detail{
   void video_palette_dac_setup()
   {
      quan::stm32::module_enable<av_dac_nsync::port_type>();
      quan::stm32::apply<
         av_dac_nsync
         , quan::stm32::gpio::mode::output
         , quan::stm32::gpio::otype::push_pull
         , quan::stm32::gpio::pupd::none
         , quan::stm32::gpio::ospeed::medium_fast
         , quan::stm32::gpio::ostate::high
      >();
      quan::stm32::module_enable<av_dac_data::port_type>();
      quan::stm32::apply<
         av_dac_data
         , quan::stm32::gpio::mode::output
         , quan::stm32::gpio::otype::push_pull
         , quan::stm32::gpio::pupd::none
         , quan::stm32::gpio::ospeed::medium_fast
         , quan::stm32::gpio::ostate::low
      >();
      quan::stm32::module_enable<av_dac_clk::port_type>();
      quan::stm32::apply<
         av_dac_clk
         , quan::stm32::gpio::mode::output
         , quan::stm32::gpio::otype::push_pull
         , quan::stm32::gpio::pupd::none
         , quan::stm32::gpio::ospeed::medium_fast
         , quan::stm32::gpio::ostate::high
      >();


      set_init_dac_values();
   }
}// detail

 #endif 
 
 
 #endif // #if !defined QUAN_AERFLITE_BOARD