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
 External DAC using DAC08S084S05 8 bit DAC. Uses soft spi to avoid use of SPI hardware
 Uses a Fifo and an interrupt.
// store new vals in fifo then update all together
// that can be done with the dac write commands!
// use a timer to create an interrupt
// say 10 usec per bit
*/
namespace {
   quan::fifo<uint16_t, 8> dac_fifo;
   uint16_t dac_data = 0;
   uint8_t bit_count = 0U;

   void delay()
   {
      asm volatile("nop":::);
   }

   void ll_dac_write(uint16_t val)
   {
     video_level_dac_irq_timer::get()->dier.bb_clearbit<0>(); //(UIE)
     dac_fifo.put(val);
     video_level_dac_irq_timer::get()->cr1.bb_setbit<0>(); //(CEN)
     video_level_dac_irq_timer::get()->dier.bb_setbit<0>(); //(UIE)
   }

   using quan::stm32::set;
   using quan::stm32::clear;
   using quan::stm32::put;

   void dac_irq()
   {
      if (bit_count == 0) {
          clear<av_dac_nsync>();
      }
      #if 1
      if ( dac_data & quan::bit<uint16_t>(15)) {
          set<av_dac_data>() ;
      } else {
          clear<av_dac_data>();
      }
      #else
      // not tested yet!
      put<av_dac_data>((dac_data & quan::bit<uint16_t>(15)) != 0) ;
      #endif
      delay();
      clear<av_dac_clk>();
      if ( bit_count != 15) {
          dac_data <<= 1;
          ++bit_count;
      }else{
          if ( !dac_fifo.is_empty()) {
                dac_data = dac_fifo.get();
          }else {
               video_level_dac_irq_timer::get()->cr1.bb_clearbit<0>(); //(CEN)
               video_level_dac_irq_timer::get()->cnt = 0;
          } 
          bit_count = 0;
          set<av_dac_nsync>();
      }
      set<av_dac_clk>();
   }
}//namespace

namespace detail{
   bool dac_busy()
   {
      return ((dac_fifo.is_empty() == false) 
      || (video_level_dac_irq_timer::get()->cr1.bb_getbit<0>() == true) );
   } 
}

namespace tim = quan::stm32::tim;
namespace {
   void dac_timer_setup()
   {
      quan::stm32::module_enable<video_level_dac_irq_timer>();
      constexpr quan::time_<int32_t>::us period {10};
      constexpr quan::frequency::Hz bus_freq
      {quan::stm32::get_module_bus_frequency<video_level_dac_irq_timer>() *2};
      constexpr auto clks = period * bus_freq;
      //tim10 on APB2 so full sys freq
      static_assert( clks == 168 *10,"error in calc");
      tim::cr1_t cr1 = 0U;
      cr1.urs = true;
      video_level_dac_irq_timer::get()->cr1.set(cr1.value);
      video_level_dac_irq_timer::get()->arr = clks;
      video_level_dac_irq_timer::get()->cnt = 0;
      video_level_dac_irq_timer::get()->sr = 0;
      // dont enable timer till needed
   //########same for all boards ######
      NVIC_SetPriority(TIM1_UP_TIM10_IRQn,interrupt_priority::video_level);
      NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
   }
}
//####################### same for all boards###############
extern "C" void TIM1_UP_TIM10_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void TIM1_UP_TIM10_IRQHandler()
{
   video_level_dac_irq_timer::get()->sr = 0;
   dac_irq();
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
      // for FMS6141 with 0.28 V d.c. offset at output with ac input
      Dac_write (dac_sync_idx, quan::voltage::V{0.58f}, 0);
      //########## FOR NTSC should be slightly above ?
      Dac_write (dac_black_idx, quan::voltage::V{0.9f}, 0); 
      Dac_write (dac_white_idx, quan::voltage::V{2.26f} , 0); 
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
       Dac_write (0b11, quan::voltage::V{0.58f}, 0);  // sync comp
       Dac_write (0b00, quan::voltage::V{0.9f}, 0); // black level
       Dac_write (0b01, quan::voltage::V{2.26f}, 0); // white
       Dac_write (0b10, quan::voltage::V{0.28f}, 1); // synctip

   }



   void set_init_dac_values()
   {
      if( osd_state::get() == osd_state::external_video){
       external_video_mode_dac_setup();  
      }else{
         internal_video_mode_dac_setup();
      }
   }
}

namespace detail{
   void video_palette_dac_setup()
   {
         /*
    For Discovery, dont use DAC2 on PA5
    Make  PE3 output High to set the LIS32DL to I2C mode
    make PA5 output low to set low clock
   */
        #if ((QUAN_OSD_BOARD_TYPE == 4 ) &&  ( defined QUAN_DISCOVERY))
         quan::stm32::module_enable<quan::stm32::gpioe>();
         quan::stm32::apply<
            quan::mcu::pin<quan::stm32::gpioe,3>
            , quan::stm32::gpio::mode::output
            , quan::stm32::gpio::otype::push_pull
            , quan::stm32::gpio::pupd::none
            , quan::stm32::gpio::ospeed::slow
            , quan::stm32::gpio::ostate::high
         >();
          quan::stm32::module_enable<quan::stm32::gpioa>();
         quan::stm32::apply<
            quan::mcu::pin<quan::stm32::gpioa,5>
            , quan::stm32::gpio::mode::output
            , quan::stm32::gpio::otype::push_pull
            , quan::stm32::gpio::pupd::none
            , quan::stm32::gpio::ospeed::slow
            , quan::stm32::gpio::ostate::low
         >();
        #endif
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

        dac_timer_setup();

        set_init_dac_values();
   }
}// detail

 #endif 
 
 
 