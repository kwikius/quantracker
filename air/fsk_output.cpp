/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */

#include "fsk_params.hpp"
#include "resources.hpp"
#include <quan/stm32/gpio.hpp>
#include <quan/utility/fifo.hpp> 
#if defined QUAN_STM32F4
#include <stm32f4xx.h>
#else
#if defined QUAN_STM32F0
#include <stm32f0xx.h>
#else
#error need to define processor
#endif
#endif
#include "fsk.hpp"

using namespace quan::stm32;

namespace {

   void dac_port_setup()
   {
      module_enable<dac_out_pin::port_type>();
      apply<
         dac_out_pin
         , gpio::mode::analog
         , gpio::pupd::none
      >();
   }

   void dac_timer_setup()
   {
      module_enable<tim6>();

      tim6::get()->cnt = 0;
   
      tim6::get()->psc = 0;
#if defined QUAN_STM32F4
// got something wrong with f4 bus frequencies calcs here!
// would suggest actual freq is 2x the value we find
      tim6::get()->arr = 2*fsk_params::clks_per_dac_write - 1;
#else
#if defined QUAN_STM32F0
       tim6::get()->arr = fsk_params::clks_per_dac_write - 1;
#endif
#endif

      tim6::get()->cr1 = (1 << 2); // (URS)
      
      tim6::get()->cr2 = 0b010 << 4 ; // trigger output on update (TRGO)
      
      tim6::get()->sr.clearbit<0>(); // (UIF)

      NVIC_SetPriority(TIM6_DAC_IRQn,interrupt_priority::fsk_dac_timer);
      NVIC_EnableIRQ(TIM6_DAC_IRQn);
      // enable update interrupt
      tim6::get()->dier.setbit<0>() ; // (UIE)

      enable<tim6>();
   }

      // the fifo for the fsk output data
   quan::fifo<uint8_t,120> output_fifo;
}
namespace fsk {
   // send data in FSK format
   bool put(char ch)
   {
   #if  QUAN_STM32_HAS_BITBANDING
      // disable TIM6 interrupt
       tim6::get()->dier.bb_clearbit<0>() ; // (UIE)
   #else
      tim6::get()->dier.clearbit<0>() ; // (UIE)
   #endif
      bool const result = output_fifo.put(static_cast<uint8_t>(ch));
      // enable TIM6 interrupt
   #if  QUAN_STM32_HAS_BITBANDING
      tim6::get()->dier.bb_setbit<0>() ; // (UIE)
   #else
      tim6::get()->dier.setbit<0>() ; // (UIE)
   #endif
      return result;
   }

   bool write( const char* buf, size_t len)
   {

      for (size_t i =0; i < len; ++i){
        fsk::put(buf[i]);
      }

   }
}

namespace {

   constexpr fsk_params::dac_data_type sin_array[/*fsk_params::num_sin_values*/] = 
   // the sine data has been precalced in the file 
   #include "sin_array.txt"

   static_assert( (sizeof(sin_array)/sizeof(fsk_params::dac_data_type)) == fsk_params::num_sin_values, "invalid array_length");

   // The index of the current waveform dac value 
   uint32_t cur_dac_value_idx = 0;

   void dac_load()
   {
#if defined QUAN_STM32F4
       DAC->DHR12RD = sin_array[cur_dac_value_idx];
#else
#if defined QUAN_STM32F0
       DAC->DHR12R1 = sin_array[cur_dac_value_idx];
#else
#error need to define processor
#endif
#endif
   }

   void advance_mark()
   {
      dac_load();
      cur_dac_value_idx = (cur_dac_value_idx + fsk_params::low_freq_steps) % fsk_params::num_sin_values;
   }

   void advance_space()
   {
      dac_load();
      cur_dac_value_idx = (cur_dac_value_idx + fsk_params::high_freq_steps) % fsk_params::num_sin_values;
   }
  
   uint8_t bit_pos = 0;
   uint8_t output_byte =0;
   uint16_t sample_idx = 0;

   enum opt_state { idle =0,start_bit=1,data_mark=2, data_space=3};
   static opt_state cur_state = opt_state::idle;

  // void ( *pf_fsk )() = in_idle;

   void mark_space()
   {
      if (++bit_pos == 8){
         advance_mark();
        // pf_fsk = in_idle;
         cur_state = opt_state::idle;
      }else {
         if ( output_byte & ( 1 << bit_pos) ){
            advance_mark();
           // pf_fsk = in_data_mark;
           cur_state = opt_state::data_mark;
         }else{
            advance_space();
            //pf_fsk = in_data_space;
            cur_state = opt_state::data_space;
         }
      }
      sample_idx = 1;
   }

   void in_data_mark()
   {
      if( ++ sample_idx < fsk_params::samples_per_bit){
         advance_mark();
      }else{
         mark_space();
      }
   }

   void in_data_space()
   {
      if( ++ sample_idx < fsk_params::samples_per_bit){
         advance_space();
      }else{
          mark_space();
      }
   }

   void in_start_bit()
   {
      if( ++ sample_idx < fsk_params::samples_per_bit){
         advance_space();
      }else{ // start bit done
         if ( output_byte & 1){ // bit 0 is a 1
            advance_mark();
           // pf_fsk = in_data_mark;
            cur_state = opt_state::data_mark;
         }else{ // bit 0 is a 0
            advance_space();
            //pf_fsk = in_data_space;
            cur_state = opt_state::data_space;
         }
         bit_pos = 0;
         sample_idx = 1;
      }
   }

   void  in_idle()
   {
      if( ++ sample_idx < fsk_params::samples_per_bit){
         advance_mark();
      }else{ // end of stop bit
         if ( output_fifo.is_empty() ){ 
            advance_mark();
         }else{
/*
            static bool led_on = true;
               if ( led_on){
                    quan::stm32::clear<heartbeat_led_pin>();
                    led_on = false;
               }else{
                    quan::stm32::set<heartbeat_led_pin>();
                    led_on = true;
               }
*/
            output_fifo.get(output_byte); // new data in
            advance_space();
           // pf_fsk = in_start_bit;
            cur_state = opt_state::start_bit;
         }
         sample_idx = 1;
      }
   }

}// namespace

void fsk::setup()
{
   // set up dac pin
   dac_port_setup();
   // enable the dac module
   quan::stm32::rcc::get()->apb1enr.setbit<29>(); //(DAC_EN)

   DAC->SR = 0;
   DAC->CR |=  (1<<2) ; // (TEN1) enable ch1 trigger  
   DAC->CR &= ~(0b111 << 3) ;//(TSEL)  update trigger is timer6 TRG0 ( default)
   // no wave generation ( default)
   // set output buffer enabled ( default)

#if defined QUAN_STM32F4
   // write initial value (0v) to reg
   DAC->DHR12RD = fsk_params::dac_max_value / 2;
#else
#if defined QUAN_STM32F0
  DAC->DHR12R1 = fsk_params::dac_max_value / 2;
#else
#error need to define processor
#endif
#endif

   DAC->CR |= 1; // (EN1) Enable dac channel 1

   dac_timer_setup();
}

extern "C" void TIM6_DAC_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void TIM6_DAC_IRQHandler()
{
#if QUAN_STM32_HAS_BITBANDING
   tim6::get()->sr.bb_clearbit<0>(); // (UIF)
#else
   tim6::get()->sr.clearbit<0>(); // (UIF)
#endif

   switch (cur_state){
      case opt_state::idle:
         in_idle();
         break;
      case opt_state::start_bit:
         in_start_bit();
         break;
      case opt_state::data_mark:
         in_data_mark();
         break;
      case opt_state::data_space:
         in_data_space();
         break;
   } 
  
}
