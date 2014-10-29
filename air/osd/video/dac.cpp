#include <stm32f4xx.h>
#include <quan/bit.hpp>
#include <quan/time.hpp>

#include <quan/frequency.hpp>
#include <quan/utility/fifo.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/constrain.hpp>
#include "../resources.hpp"

#if (QUAN_OSD_BOARD_TYPE == 2 )  || (QUAN_OSD_BOARD_TYPE == 3 )

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
  dac_irq_timer::get()->dier.bb_clearbit<0>(); //(UIE)
  dac_fifo.put(val);
  dac_irq_timer::get()->cr1.bb_setbit<0>(); //(CEN)
  dac_irq_timer::get()->dier.bb_setbit<0>(); //(UIE)
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
               dac_irq_timer::get()->cr1.bb_clearbit<0>(); //(CEN)
               dac_irq_timer::get()->cnt = 0;
          } 
          bit_count = 0;
          set<av_dac_nsync>();
     }
     set<av_dac_clk>();
}
}//namespace
namespace tim = quan::stm32::tim;
void dac_timer_setup()
{
     quan::stm32::module_enable<dac_irq_timer>();
     constexpr quan::time_<int32_t>::us period {10};
     constexpr quan::frequency::Hz bus_freq
     {quan::stm32::get_module_bus_frequency<dac_irq_timer>() *2};
     constexpr auto clks = period * bus_freq;
     //tim10 on APB2 so full sys freq
     static_assert( clks == 168 *10,"error in calc");
     tim::cr1_t cr1 = 0U;
     cr1.urs = true;
     dac_irq_timer::get()->cr1.set(cr1.value);
     dac_irq_timer::get()->arr = clks;
     dac_irq_timer::get()->cnt = 0;
     dac_irq_timer::get()->sr = 0;
     // dont enable timer till needed
     NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

extern "C" void TIM1_UP_TIM10_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void TIM1_UP_TIM10_IRQHandler()
{
      dac_irq_timer::get()->sr = 0;
      dac_irq();
}

namespace {

   constexpr quan::voltage::V vref{3.28};
   constexpr quan::voltage::V ze{0.005};
   constexpr float fse = 0.0075;
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
   void set_init_dac_values()
   {
    #if (QUAN_OSD_BOARD_TYPE == 2 )  || (QUAN_OSD_BOARD_TYPE == 3 )
      
       serial_port::write("OSD startup\n");

       constexpr uint8_t dac_sync_idx = 0;
       constexpr uint8_t dac_black_idx = 1;
       constexpr uint8_t dac_white_idx = 2;
       constexpr uint8_t dac_data_idx = 3;

   //0.16 too low
   //0.18 worksbut not great
   // 0.2 works but not great
   // 0.24 works but not great
   // 0.26 ok but not at bottom
   //0.27
   //0.28 look v good at top bu not bottom
   //0.29
   //0.3 too high

      // Dac_write (dac_sync_idx, quan::voltage::V{0.4f}, 0);
       Dac_write (dac_black_idx, quan::voltage::V{0.64f}, 0); // 0.64
       Dac_write (dac_white_idx, quan::voltage::V{2.04f} , 0); // 2.04
       Dac_write (dac_data_idx, quan::voltage::V{1.2f}, 1);
   #endif
   }
}

void Dac_setup()
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

     dac_timer_setup();

     set_init_dac_values();
}

 #endif 
 
 
 