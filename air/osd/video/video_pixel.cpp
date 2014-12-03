#include <stm32f4xx.h>
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include "video_cfg.hpp"
// OSD pixel clock
uint16_t video_cfg::spi_clock::m_timer_half_clks_per_px = 6;   // 14 MHz px clk
// Data bit clock
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
         ,quan::stm32::gpio::mode::af3  // TIM9_CH1
#else
         ,quan::stm32::gpio::mode::af1  // TIM1_CH1
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
            cr1.cms  = 0b00;   // edge aligned counting
            cr1.dir  =  false; // up counting
            cr1.urs  = true;   // only update event generates update interrupt
            cr1.opm  = false;  // not one pulse mode
            cr1.udis = false;
            cr1.cen  = false;
         timer::get()->cr1.set(cr1.value);
      }
      {
         quan::stm32::tim::cr2_t cr2 = timer::get()->cr2.get();
            cr2.ti1s = false;
            cr2.mms  = 0b000;   // master mode is dont care
            cr2.ccds = false;  // dont care
            cr2.ois1 = false;  // OC1 low when inactive
         timer::get()->cr2.set(cr2.value);
      }
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

