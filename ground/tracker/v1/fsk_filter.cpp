/*
 Copyright (c) 2014 Andy Little 

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
#include <quan/frequency.hpp>
#include <quan/stm32/rcc.hpp>
#include <quan/stm32/push_pop_fp.hpp>
#include <quan/stm32/detail/get_bus.hpp>
#include <quan/stm32/sys_freq.hpp>
#include "resources.hpp"
#include "serial_ports.hpp"

/*
from "Simplified FSK Signal Detection"
 by Dennis Sequine
which appeared in Circuit Cellar issue 194, Sept 2006.
*/

namespace {

//########################mkfilter##############################
/* Digital filter designed by mkfilter/mkshape/gencode A.J. Fisher
 http://www-users.cs.york.ac.uk/~fisher/mkfilter

 Butterworth low pass
 corner freq = 760 Hz
 sample freq = 53.5714285714 kHz
*/
float lp_filter (float in)
{
   constexpr uint32_t n_lp_zeros = 2;
   constexpr uint32_t n_lp_poles = 2;
   constexpr float recip_gain = 1.f / 5.354740286e+02f;

   static float lp_xv[n_lp_zeros+1];
   static float lp_yv[n_lp_poles+1];

   lp_xv[0] = lp_xv[1];
   lp_xv[1] = lp_xv[2];
   lp_yv[0] = lp_yv[1];
   lp_yv[1] = lp_yv[2];
   
   lp_xv[2] = in * recip_gain;
   lp_yv[2] = (lp_xv[0] + lp_xv[2]) + 2.f * lp_xv[1]
              + (-0.8815626574f * lp_yv[0]) + (1.8740926406f * lp_yv[1]);
   return lp_yv[2];
}

//#######################mkfilter###########################
/* Digital filter designed by mkfilter/mkshape/gencode A.J. Fisher
 http://www-users.cs.york.ac.uk/~fisher/mkfilter

 Bessel Bandpass 
 low cutoff freq = 600, 
 high cutoff freq = 4200
 sample freq = 53.5714285714 kHz
*/
float bandpass_filter (float in)
{
   constexpr uint32_t n_bp_zeros = 4;
   constexpr uint32_t n_bp_poles = 4;
   constexpr float recip_gain = 1.f / 1.822385761e+01f;

   static float bp_xv[n_bp_zeros+1];
   static float bp_yv[n_bp_poles+1];

   bp_xv[0] = bp_xv[1];
   bp_xv[1] = bp_xv[2];
   bp_xv[2] = bp_xv[3];
   bp_xv[3] = bp_xv[4];
   bp_yv[0] = bp_yv[1];
   bp_yv[1] = bp_yv[2];
   bp_yv[2] = bp_yv[3];
   bp_yv[3] = bp_yv[4];
   
   bp_xv[4] = in * recip_gain;
   bp_yv[4] = (bp_xv[0] + bp_xv[4]) - 2 * bp_xv[2]
              + (-0.3689615189f * bp_yv[0]) + (1.8619306759f * bp_yv[1])
              + (-3.5946003295f * bp_yv[2]) + (3.1007745276f * bp_yv[3]);
   return bp_yv[4];
}

bool output_state = true;
bool signal_state = false;

// whether there appears to be a valid signal
inline bool have_signal()
{
   return signal_state;
}

//output currently high (true) or low
inline bool get_output_state()
{
   return output_state;
}

// set 1200 baud serial tx output hi
inline void set_fsk_demod_output()
{
   // set output high on match
   auto ccmr1_reg = fsk_filter_timer::get()->ccmr1.get();
      
      ccmr1_reg  &= ~ (0b111 << 12);  // (OC2M 14:12)
      ccmr1_reg  |= (0b001 << 12);
   fsk_filter_timer::get()->ccmr1 = ccmr1_reg;

   output_state = true;
}

// set 1200 baud serial tx output lo
inline void clear_fsk_demod_output()
{
   // set output low on match
 auto ccmr1_reg = fsk_filter_timer::get()->ccmr1.get();
     
      ccmr1_reg  &= ~ (0b111 << 12);  // (OC2M 14:12)
      ccmr1_reg  |= (0b010 << 12);
   fsk_filter_timer::get()->ccmr1 = ccmr1_reg;

   output_state = false;
}
 
// fsk demod for bell202
inline void fsk_demod()
{
   // virtual 0v half way between GND and VDD
   // will be adjusted as input comes in
   // near zero
   static float zero_volts_rail = 2047.f;
   constexpr float zero_v_rail_filter_val = 0.001f;
   constexpr float sig_level_filter_val = 0.01f;
   
   static float average_signal_level = 0.f;
   // set min signal level to distinguish valid signal from noise
   // 2 levels fopr hysteresis , maybe make adjustable
   constexpr float min_signal_level_on = 40.f; // maybe 50 mV rms
   constexpr float min_signal_level_off = 30.f; // maybe 40 mV rms
   constexpr float output_hysteresis = 0.1f;

   static uint32_t shift_reg = 0x00FFFFFF;
   /// start...
   // get latest sample (clears the A2D interrupt flag)
   float const raw_a2d = static_cast<float> (ADC1->DR);
   //refer to 0v and apply input bandpass filter
   
  // float const wave_val = bandpass_filter (raw_a2d - zero_volts_rail);
   float const wave_val = raw_a2d - zero_volts_rail;
   
   float const abs_wave_val = (wave_val >= 0.f)
      ? wave_val
      :-wave_val;
   // simple exponentially weighted moving average
   // lp filter for the virtual 0v rail, to be the average of all the inputs
   // since the actual input is based on a voltage divider of 2 resistors
   //  which isnt that accurate
   zero_volts_rail = (zero_volts_rail * (1.f - zero_v_rail_filter_val)) + (raw_a2d * zero_v_rail_filter_val);
   //also want to know average signal level, use exponentially weighted moving average here too
   average_signal_level = (average_signal_level * (1.f - sig_level_filter_val)) + (sig_level_filter_val * abs_wave_val);
   // set signal on or off according to signal level with hysteresis..
   if (!have_signal()) {
      if (average_signal_level > min_signal_level_on) {
         signal_state = true;
        // quan::stm32::set<blue_led_pin>();
      }
   } else {
      if (average_signal_level < min_signal_level_off) {
         signal_state = false;
        // quan::stm32::clear<blue_led_pin>();
      }
   }
   // The correlator ... first digitalize the input
   uint32_t const input_bit = (wave_val >= 0.f) ? 1U:0U;
   // XOR the latest digitalised sample with the digitalized sample from 448 usec ago
   bool const raw_output_bit = ( (shift_reg & 1U) ^ input_bit) != 0U;
   // add the latest digitalised sample to 24 bit shift reg
   // and shift everything along
   shift_reg = (shift_reg >> 1U) | (input_bit << 23U) ;
   // analogise the correlator output and...
   float const filter_input = (raw_output_bit == true) ? 1.0f: -1.0f;
   // ...lp filter the correlator output
   float const filtered_output = lp_filter (filter_input);
   if (have_signal()) { // signal ok so output data
      if (get_output_state()) {
         // use hysteresis on the output to prevent jitter
         if (filtered_output < -output_hysteresis) {
            clear_fsk_demod_output();
         }
      } else {
         if (filtered_output > output_hysteresis) {
            set_fsk_demod_output();
         }
      }
   } else { // no signal detected so set to mark state
      set_fsk_demod_output();
   }
}
 
} // namespace
 
// best set to high priority
extern "C" void ADC_IRQHandler() __attribute__ ( (interrupt ("IRQ")));
// routine takes max 300 cycles
// so say 1.8 usec in every 18 usec 10% of cpu time!
extern "C" void ADC_IRQHandler()
{
   quan::stm32::push_FPregs();
   fsk_demod();
   quan::stm32::pop_FPregs();
}
 
namespace {
   void setup_fsk_demod_adc()
   {
   // set up ADCCLK via APB2_ENR
      static constexpr uint8_t apb2enr_adc1en = 8;
      quan::stm32::rcc::get()->apb2enr.setbit<apb2enr_adc1en>();
   // TODO add reset ADC in RCC
   //###
      // ADC Clk max = 36 MHz
      // PCLK2 = SysClk/2 = 84 MHz
      // set prescaler = /4 -> 21 MHz
      ADC->CCR = (ADC->CCR & ~ (0b11 << 16)) | (0b01 << 16); // (ADCPRE 17:16)
      // turn off the ADC
      ADC1->CR2 &= ~ (1 << 0) ; //(ADON)
      // Select Channel 11 only     ADC123_IN11
      ADC1->SQR3 = 11;
      // say want 1 conv in group  L <- 0 for 1 conv
      ADC1->SQR1 &= ~ (0b1111 << 20); //( L 23:20)
      //ADC1 channel 11 set to sample time
      // set 56 ADC cycles
      // adc clock of 21 MHz gives sampling time of 2.6667 usec
      // which is 0.5% of high freq cycle
      // total conv time = 56 + 12 = 68 ADC cycles == 3.23 usec
      ADC1->SMPR1 = (ADC1->SMPR1 & ~ (0b111 << 3)) | (0b011 << 3) ; // (SMP11 5:3)
      auto adc_cr1 = ADC1->CR1;
         // no scan mode
         adc_cr1 &= ~ (1 << 8); // (SCAN)
         // want end of conversion interrupt
         adc_cr1 |= (1 << 5);    // (EOCIE)
         // want 12 bit resolution
         adc_cr1 &= ~ (0b11 << 24) ; // (RES 25:24)
         // dont want discontinuous mode
         adc_cr1 &= ~ (1 << 11);  // (DISCEN)
      ADC1->CR1 = adc_cr1;
      
      // set 1 channel conversion ( default)

      // want single conversion per trigger
      auto adc_cr2 = ADC1->CR2;
         adc_cr2 &= ~ (1 << 1); // (CONT)
         // want right alignment ( lsb is bit 0)
         adc_cr2 &= ~ (1 << 11) ; // (ALIGN)
         //want external trigger rising edge
         adc_cr2 &= ~ (0b11 << 28); // (EXTEN 29:28)
         adc_cr2 |= (0b10 << 28);  // (EXTEN 29:28)
         // select Timer 4 CC4 as trigger
         adc_cr2 &= ~ (0b1111 << 24); // (EXTSEL 27:24);
         adc_cr2 |= (0b1001 << 24);  // (EXTSEL 27:24);
         // clear any status flags
         ADC1->SR = 0;
         // dont forget the NVIC!
         NVIC_EnableIRQ (ADC_IRQn);
         // turn on the A2D
         adc_cr2 |= (1 << 0) ;  //(ADON)
      ADC1->CR2 = adc_cr2;
   }
   // setup timer to give interrupt every 18.667 usec for demod
   // Timer4 channel 4
   // Timer4 Channel 2
   void setup_fsk_demod_timer()
   {
      quan::stm32::disable<fsk_filter_timer>(); // Clear CR1.CEN
      quan::stm32::module_enable<fsk_filter_timer>();
      quan::stm32::module_reset<fsk_filter_timer>();
      // no prescaler to give max resolution
      fsk_filter_timer::get()->psc = static_cast<uint16_t> (0);
      // get clk freq
      typedef quan::stm32::detail::get_bus<fsk_filter_timer>::type tbus;
      
      constexpr uint32_t freq = quan::stm32::get_bus_frequency<tbus>();
      static_assert (freq == 42000000,"unexpected freq");
      static uint16_t count = 784 *2 ; // *2 is consistent but not sure why
      fsk_filter_timer::get()->arr = count - 1;
      fsk_filter_timer::get()->cnt = 0;
      auto cr1_reg = fsk_filter_timer::get()->cr1.get();
         typedef decltype (cr1_reg) cr1_vt;
         
         // no clock division
         cr1_reg &= ~ (quan::bit<cr1_vt> (9) | quan::bit<cr1_vt> (8)); //(CKD[1:0])
         // preload the auto reload cr1_reg permanently to give a constant period
         // now the ARR has been written above
         cr1_reg &= ~quan::bit<cr1_vt> (7) ; // (ARPE)
         // set edge aligned counting
         cr1_reg &= ~ (quan::bit<cr1_vt> (6) | quan::bit<cr1_vt> (5)); //(CMS[1:0])
         // set counting direction up
         cr1_reg &= ~quan::bit<cr1_vt> (4); // (DIR)
         // Not one pulse mode
         cr1_reg &= ~quan::bit<cr1_vt> (3); // (OPM)
         //only counter overflow generates an update irq or DMA request if enebled
         cr1_reg |= quan::bit<cr1_vt> (2); // (URS)
         // disable update event
         // NOTE: may want update event enabled to load new value in CC cr1_reg
         cr1_reg |= quan::bit<cr1_vt> (1); // (UDIS)
      
      fsk_filter_timer::get()->cr1 = cr1_reg;
      
      fsk_filter_timer::get()->sr = 0;
      //  interrupts and DMA
      fsk_filter_timer::get()->dier = 0U;
      // no interrupts r DMA
      // fsk_filter_timer::get()->dier |= (1 << 0); //(UIE)
      // fsk_filter_timer::get()->dier |= (1 << 4); // (CC4IE)
   //####################
      // egr - software event generation not needed
      fsk_filter_timer::get()->egr = 0U;
   //###########################
      //  slave mode disabled - timer counts using internal clock
      // (other flags in smcr all relate to other modes, so dont care)
      fsk_filter_timer::get()->smcr &= ~static_cast<uint32_t> (0b111); // ( SMS[2:0])
      // need to setup Compare ch 4
      auto ccmr2_reg = fsk_filter_timer::get()->ccmr2.get();
      // set output mode
         ccmr2_reg  &= ~ (0b11 << 8U); // (CC4S 9:8)
         // set pwm mode 1 PWM mode active when timer < val_in_reg
         // rising edge of OCR4REF
         ccmr2_reg &= ~ (0b111 << 12U); // (OC4M 14:12)
         ccmr2_reg |= (0b110 << 12U);
         // disable CC4 preload
         ccmr2_reg &= ~ (1 << 11U);

      fsk_filter_timer::get()->ccmr2 = ccmr2_reg;
      // output must be enabled even though no pin is assigned
      // in order to get the ADC trigger
      fsk_filter_timer::get()->ccr4 = 84U; // gives approx 2 usec pulse for adc start
      fsk_filter_timer::get()->ccer |= (1 << 12); // CC4E
      // setup Compare Ch2 for data output
      auto ccmr1_reg = fsk_filter_timer::get()->ccmr1.get();
         // set output mode
         ccmr1_reg  &= ~(0b11 << 8U); // (CC2S 9:8)
        // set output forced high to start
         ccmr1_reg  &= ~(0b111 << 12);  // (OC2M 14:12)
         ccmr1_reg  |= (0b101 << 12);
         // disable CC2 preload
         ccmr1_reg &= ~(1 <<11U);
      fsk_filter_timer::get()->ccmr1 = ccmr1_reg;
      fsk_filter_timer::get()->ccr2 = count - 2; // longest possible delay
      // enable demod tx output
      fsk_filter_timer::get()->ccer |= (1 << 4); // CC2E
      // could set?
      fsk_filter_timer::get()->cr1 &= ~quan::bit<cr1_vt> (1); // (UDIS)
      // enable filter_timer...
      quan::stm32::enable<fsk_filter_timer>(); // set CR1.CEN
   }
 
} // namespace
 
void setup_fsk_demod()
{
   quan::stm32::module_enable<av_telem_raw_fsk_in_plus::port_type>();
   
   quan::stm32::apply<
      av_telem_raw_fsk_in_plus
      ,quan::stm32::gpio::mode::analog
      ,quan::stm32::gpio::pupd::none
   >();

   // setup data out using TIM4_CH2
   quan::stm32::module_enable<av_telem_fsk_demod_out::port_type>();
   quan::stm32::apply<
      av_telem_fsk_demod_out
      ,quan::stm32::gpio::mode::af2
      ,quan::stm32::gpio::pupd::none
      ,quan::stm32::gpio::ospeed::slow
   >();
   
   setup_fsk_demod_adc();
   setup_fsk_demod_timer();
   av_fsk::serial_port::set_irq_priority(interrupt_priority::fsk_adc);
}
