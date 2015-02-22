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

#include <stm32f4xx.h>
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include "video_cfg.hpp"
// OSD pixel clock basically controlled by reqd number of pixels per line
// Digital video formats
// PAL 720 * 576
// NTSC 720 * 486
// visible_line_time = video_cfg::get_visible_line_period();
// num_pixels_per_visible_line =(interlaced)?768:768/2 or 720?
// timerclks_px = visible_line_time * bus_clk_freq / num_pixels_per_visible_line
// use half as uses pwm so needs an integer for each half cycle
#if defined (QUAN_DISPLAY_INTERLACED)
uint16_t video_cfg::spi_clock::m_timer_half_clks_per_px = 6;   // 14 MHz px clk
#else
uint16_t video_cfg::spi_clock::m_timer_half_clks_per_px = 12;   // 7 MHz px clk
#endif
// Data bit clock
// timr_clks_bit = visible_line_time * bus_clk_freq / num_bits_per_visible_line
// should calc that to baud
// num_bits_per_visible_line = visible_line_time * baud
// timer_clks_bit = visible_line_time * bus_clk_freq / num_bits_per_visible_line
// use half as uses pwm so needs an integer for each half cycle
uint16_t video_cfg::spi_clock::m_timer_half_clks_per_bit = 42; //  2 MHz bit clk

 void video_cfg::spi_clock::setup()
   {
 // if boardtype == 4 spi_clock output == TIM9_CH1
 // else
      //spi_clock output == TIM1_CH1
 
      quan::stm32::module_enable<video_spi_clock::port_type>();
      quan::stm32::apply<
         video_spi_clock
#if (QUAN_OSD_BOARD_TYPE == 4)
         ,quan::stm32::gpio::mode::af3  //PA2 TIM9_CH1
#else
         ,quan::stm32::gpio::mode::af1  // PA8 TIM1_CH1
#endif
         ,quan::stm32::gpio::ospeed::fast
         ,quan::stm32::gpio::ostate::low
      >();

      quan::stm32::module_enable<timer>();
      quan::stm32::module_reset<timer>();
      {
// may be different for tim9
         quan::stm32::tim::cr1_t cr1 = timer::get()->cr1.get();
            cr1.ckd  = 0b000;  // no clk division
            cr1.arpe = false;   // auto reload is buffered
#if (QUAN_OSD_BOARD_TYPE != 4)
// only on tim1
            cr1.cms  = 0b00;   // edge aligned counting
            cr1.dir  =  false; // up counting
#endif
            cr1.urs  = true;   // only update event generates update interrupt
            cr1.opm  = false;  // not one pulse mode
            cr1.udis = false;
            cr1.cen  = false;
         timer::get()->cr1.set(cr1.value);
      }
#if (QUAN_OSD_BOARD_TYPE != 4)
// no cr2 on tim9
      {
         quan::stm32::tim::cr2_t cr2 = timer::get()->cr2.get();
            cr2.ti1s = false;
            cr2.mms  = 0b000;   // master mode is dont care
            cr2.ccds = false;  // dont care
            cr2.ois1 = false;  // OC1 low when inactive
         timer::get()->cr2.set(cr2.value);
      }
#endif
      {
         quan::stm32::tim::smcr_t smcr = timer::get()->smcr.get();
           //To TRY--> smcr.msm = true;
            smcr.etp = false;  // dont care
            smcr.ece = false;  // not external clock
            smcr.etps = 0b00;  // no prescale
            smcr.etf = 0b000 ; // dont care
#if (QUAN_OSD_BOARD_TYPE == 4)
            smcr.ts = 0b000;   // TIM9 slave ITR1 trigger from TIM2
#else
            smcr.ts  = 0b001;   // TIM1 slave ITR1 trigger from TIM2
#endif
            smcr.sms = 0b101;  // input trigger Gated mode
            // smcr.sms = 0b000; // (test clock waveform)
         timer::get()->smcr.set(smcr.value);
      }
      {
         quan::stm32::tim::ccmr1_t ccmr1 = timer::get()->ccmr1.get();
           ccmr1.cc1s = 0b00;   // channel 1 output
           ccmr1.oc1m = 0b111;  // PWM mode 2
           ccmr1.oc1pe = false;
           ccmr1.oc1fe = false;
         timer::get()->ccmr1.set(ccmr1.value);
      }
      {
         quan::stm32::tim::ccer_t ccer = timer::get()->ccer.get();
            ccer.cc1e = true; // enable output
            ccer.cc1p = false; // clock active high
         timer::get()->ccer.set(ccer.value);
      }
      #if (QUAN_OSD_BOARD_TYPE != 4) 
       timer::get()->rcr = 0;
      #endif
       // set pwm period and 50% duty cycle
       timer::get()->arr = get_timer_clks_per_px()-1;
       timer::get()->ccr1 = (get_timer_clks_per_px()/2 )-1;
 
       timer::get()->cnt = 0;
       timer::get()->sr = 0;
       timer::get()->dier.set(0);// no irqs or dma
 // no bdtr on TIM9
      #if (QUAN_OSD_BOARD_TYPE != 4)
       quan::stm32::tim::bdtr_t bdtr = timer::get()->bdtr.get();
         bdtr.aoe = false; 
         bdtr.bkp = false;  // dont care
         bdtr.bke = false; // break disabled
         bdtr.ossr = true; //  not sure
         bdtr.lock = 0b00; // lock is off
         bdtr.dtg = 0; // dont care
         bdtr.moe = true; // OC1 is on
       timer::get()->bdtr.set(bdtr.value); 
      #endif 
       //go .. Need CEN for gated mode
       timer::get()->cr1 |= ( 1<< 0); // (CEN)
   }

