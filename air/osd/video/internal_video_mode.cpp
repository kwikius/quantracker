
#include <stm32f4xx.h>
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/voltage.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/stm32/tim.hpp>
#include "video_cfg.hpp"
// assume that OSD is suspended
// external pullups may need to be removed
// since SPI pins are inputs before transmission
// so will have to control init state via internal pullups

void Dac_write( uint8_t dacnum, quan::voltage::V const & vout, uint8_t code);

namespace {

   typedef video_rows_line_counter sync_timer;

   void ivm_dac_setup()
   {
       
      // black white and sync voltages and functions, 
      // leave current csync to catch video input if and when
//       constexpr uint8_t dac_data_idx = 0; // (also grey)
//       constexpr uint8_t dac_white_idx = 1;
//       constexpr uint8_t dac_black_idx = 2;
//       constexpr uint8_t dac_sync_idx = 3;
//##########################
       // for FMS6141 with 0.28 V d.c. offset at output with ac input
      // Dac_write (dac_sync_idx, quan::voltage::V{0.58f}, 0);
//########## FOR NTSC should be slightly above ?
//    normal values
//       Dac_write (dac_black_idx, quan::voltage::V{0.9f}, 0); 
//       Dac_write (dac_white_idx, quan::voltage::V{2.26f} , 0); 
//       Dac_write (dac_data_idx, quan::voltage::V{1.58f}, 1);
// 00 (0) dac is black_level
// 01 (1) dac is white
// 10 (20 dac sync_tip
       // Dac_write (0b11, quan::voltage::V{0.58f}, 0);  // sync comp
          Dac_write (0b00, quan::voltage::V{0.9f}, 0); // black level
          Dac_write (0b01, quan::voltage::V{2.26f}, 0); // white
          Dac_write (0b10, quan::voltage::V{0.28f}, 1); // synctip

   }

   void ivm_pin_setup()
   {
      // PC6 connected to TIM3_CH1  alternate function af2
      quan::stm32::module_enable<av_telem_tx::port_type>();
      quan::stm32::apply<
         av_telem_tx
         , quan::stm32::gpio::mode::af2
         , quan::stm32::gpio::otype::push_pull
         , quan::stm32::gpio::pupd::pull_down
         , quan::stm32::gpio::ospeed::slow
         , quan::stm32::gpio::ostate::low
      >();
      // PC11 as input no pullup or down
      quan::stm32::module_enable<video_mux_out_white_miso::port_type>();
      quan::stm32::apply<
         video_mux_out_white_miso
         , quan::stm32::gpio::mode::input
         , quan::stm32::gpio::pupd::none
      >();

     // also spi clocks
      // black spi is now actually white!
      quan::stm32::module_enable<video_mux_out_black_miso::port_type>();
      quan::stm32::apply<
         video_mux_out_black_miso  // PB14 or PC2 on boardtype 4
         ,quan::stm32::gpio::mode::af5  // same for both pins
         ,quan::stm32::gpio::otype::push_pull
         ,quan::stm32::gpio::pupd::pull_down
         ,quan::stm32::gpio::ospeed::fast
         ,quan::stm32::gpio::ostate::low
      >();

   }

   void ivm_timer_setup()
   {
      // disable the timer ( as line_counter)
      sync_timer::get()->cr1.bb_clearbit<0>(); // (CEN)
      quan::stm32::module_reset<sync_timer>();
      {
         quan::stm32::tim::cr1_t cr1 = 0u;//sync_timer::get()->cr1.get();
            cr1.ckd = 0b00;  // no filter on ck
            cr1.arpe = false; // no preload
            cr1.cms = 0b00;  // counting dependednt on dir bit
            cr1.dir = false; // up counting
            cr1.opm = false; // not one pulse mode
            cr1.urs = false; 
            cr1.udis = false;
            cr1.cen = false; // counter disabled for now
         sync_timer::get()->cr1.set(cr1.value);
      }
      {
         quan::stm32::tim::cr2_t cr2 = 0U;//sync_timer::get()->cr2.get();
            cr2.ti1s = false; // single trigger
            cr2.mms = 0b100;  // OC1REF is TRGO for gate timer when enabled
            cr2.ccds = true;  // DMA on update
         sync_timer::get()->cr2.set(cr2.value);   
      }
      {
         quan::stm32::tim::smcr_t smcr = 0U;// sync_timer::get()->smcr.get();
            smcr.msm = false; // dont care
            smcr.ts = 0b000;  // dont care
            smcr.sms = 0b000; // slave mode disabled 
         sync_timer::get()->smcr.set(smcr.value);  
      }
      {
         quan::stm32::tim::dier_t dier = 0U;// sync_timer::get()->dier.get();
            dier.uie = true;
         sync_timer::get()->dier.set(dier.value);
      }
 
      sync_timer::get()->sr.set(0); 
      sync_timer::get()->egr.set(0); 
      {
         quan::stm32::tim::ccmr1_t ccmr1 = 0U;// sync_timer::get()->ccmr1.get();
            ccmr1.cc1s = 0b00; // cc1 is output mapped to PC6
            ccmr1.oc1fe = false; // dont care relates to trigger input
            ccmr1.oc1pe = false ; // new value in Capture compare reg (ccr1) is loaded direct
            ccmr1.oc1m = 0b110 ;// pwm mode 1 sync pulse at start of period
         sync_timer::get()->ccmr1.set(ccmr1.value);
            // is 'active' part of pulse
      }
      // ccmr2 not used
      //
      {
         quan::stm32::tim::ccer_t ccer = 0U; //sync_timer::get()->ccer.get();
         ccer.cc1e = true; // enable PC6 as output
         ccer.cc1p = false; // active high
         ccer.cc1np = false; // must be cleared when output
         sync_timer::get()->ccer.set(ccer.value);
      }
   }

   
   constexpr uint32_t timer_freq = quan::stm32::get_raw_timer_frequency<sync_timer>();
   constexpr uint32_t clocks_usec = timer_freq / 1000000U;

   constexpr uint16_t full_line = static_cast<uint16_t>(64U * clocks_usec);
   constexpr uint16_t half_line = static_cast<uint16_t>(32U * clocks_usec);
   
   constexpr uint16_t long_sync = (47U * clocks_usec) / 10U 
         + ((((47U * clocks_usec) % 10U )>= 5 ) ? 1: 0);

   constexpr uint16_t short_sync = (235U * clocks_usec) / 100U 
         + ((((235U * clocks_usec) % 100U )>= 500 ) ? 1: 0);

   enum ivm_mode_t{
         end_of_frame,
         pre_equalise,
         vsync,
         post_equalise,
         video_fields
   };

   ivm_mode_t ivm_mode = video_fields;
   bool first_field = true;
   uint32_t ivm_count = 0;

} //namespace

void internal_video_mode_setup()
{
   // disable interrupts from synch if necessary
   // PA15 TIM2_CH1 TIM2_ETR // 
   // PB14 TIM12_CH1   // sync_sep these should have been done
   // PB15 TIM12_CH2   // sync sep these should have been done
   // PD2  TIM3_ETR
    
   ivm_mode = video_fields;
   ivm_count = 0;
   first_field = true;
   ivm_dac_setup();
   ivm_pin_setup();
   ivm_timer_setup();
   // set initial timer values
   // get immediate start on overflow
   sync_timer::get()->cnt = 0xffff;
 
   sync_timer::get()->ccr1 = long_sync;

   sync_timer::get()->arr = (first_field ?half_line:full_line) ;
   quan::stm32::enable<sync_timer>();
   
}


// uif
void do_internal_video_mode_irq()
{
    
    sync_timer::get()->sr.set(0);
//   switch (ivm_mode){
//
//      case video_fields:
//         sync_timer::get()->arr = 
         
      
   /* line period (H) = 64 usec
    hsync 4.7 us +-0.1 us
    do  1 full rows 4.7 usec low pulse then 64 - 4.7 usec high
    just do pal for the mo

      do_end_of_frame ()
      {
       if (first field){
           period  = 32 usec  
         }else{
            period = 64 usec
         }
         do 4.7 usec low pulse ;
      }
      pre_equalise()
      {
         period = 32 usec
         for ( i = 0 to 4){ // 5 x
            do 2.35 usec low pulse
         }
      }
      vsync ()
      for ( i = 0 to 4)
      {
         period = 32 usec
         low pulse = 32 - 2.35 usec
      }
   
      post_equalise()
      {
         if ( first field){
         for ( i = 0 to 4){
              period 32 usec
              pulse 2.35 usec
            }
         }else{ // second field
            for ( i = 0 to 3){
              period 32 usec
              pulse 2.35 usec
            }
            once{
              period 64 usec
              pulse 2.35 usec
            }
         }
      }

      video_fields()
      {
         // if 
         for  (i = 0 to 305){
            period 64 usec
            pulse 4.7 usec

            do enable disable of telem and osd funs from here
         }
      }
    */
}
