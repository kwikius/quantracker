
#include <stm32f4xx.h>
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/voltage.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/stm32/tim.hpp>
#include "video_cfg.hpp"
#include "osd_state.hpp"

// assume that OSD is suspended
// external pullups may need to be removed
// since SPI pins are inputs before transmission
// so will have to control init state via internal pullups
// nwhite == adg addr0
// nblack == adg addr1
// in external mode
      
//   nblack nwhite adg704_addr  colour_out
//    0       0    0             grey
//    0       1    1             black
//    1       0    2             white  
//    1       1    3             video
// in internal mode
// nwhite is always 0
//   nblack nwhite adg704_addr  int colour_out  (ext colour_out
//    0       0     0           black           grey
//    0       1     0           black           black
//    1       0     2           white           white
//    1       1     2           white           transparent
// preset the nblack buffer to the background colour
// which is 0 rather than 1 for external mode
// only need to use the nblack buffer in internal mode
// can be white or black sy black for now
// so 
//  Transparent is never written could make sure that in set_pixel_raw
// only effect is that set transparent will set white


void Dac_write( uint8_t dacnum, quan::voltage::V const & vout, uint8_t code);

namespace {
   typedef video_rows_line_counter sync_timer;
   // line counter but not used as such in internal mode
   void ivm_timer_setup()
   {
      
      // disable the timer ( as line_counter)
      sync_timer::get()->cr1.bb_clearbit<0>(); // (CEN)
      quan::stm32::module_reset<sync_timer>();
      {
         quan::stm32::tim::cr1_t cr1 = 0u;//sync_timer::get()->cr1.get();
            cr1.ckd = 0b00;  // no filter on ck
            cr1.arpe = false; // no preload
            cr1.cms = 0b00;  // counting dependent on dir bit
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
            cr2.mms = 0b011;  // TRGO  is CC1IF to start gate_timer
            //cr2.mms = 0b100;  // OC1REF is TRGO for gate timer when enabled
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
      }
      // ccmr2 not used
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
         pre_equalise,
         vsync,
         post_equalise,
         video_fields
   };

#if 0
   constexpr uint32_t start_of_telemetry_rows = 3;
   constexpr uint32_t end_of_telemetry_rows = 16;
   constexpr uint32_t start_of_osd_rows = 17;
   constexpr uint32_t end_of_osd_rows =200;
   constexpr uint32_t num_video_fields = 304;
#else
   // for debugging
   constexpr uint32_t start_of_telemetry_rows = 1;
   constexpr uint32_t end_of_telemetry_rows = 2;
   constexpr uint32_t start_of_osd_rows = 3;
   constexpr uint32_t end_of_osd_rows = 4;
   constexpr uint32_t num_video_fields = 5;
#endif

   ivm_mode_t ivm_mode = video_fields;
   bool first_field = true;
   uint32_t ivm_count = num_video_fields - 1;

} //namespace

namespace detail{

   void sync_sep_setup();
   void sync_sep_enable();

   void internal_video_mode_setup()
   {
     // typedef video_rows_line_counter sync_timer;
      ivm_mode = video_fields;
//##############################
      first_field = true;
//##########################
      ivm_count = num_video_fields - 1;
      
    //  ivm_dac_setup();
     // ivm_pin_setup();
      ivm_timer_setup();
      // set initial timer values
      // get immediate start on overflow
      sync_timer::get()->cnt = 0xffff;
    
      sync_timer::get()->ccr1 = long_sync;

      sync_timer::get()->arr = full_line ;

      NVIC_SetPriority(TIM3_IRQn,interrupt_priority::video);
      NVIC_EnableIRQ (TIM3_IRQn);
     // sync_sep_setup();
     // sync_sep_enable();
      //quan::stm32::enable<sync_timer>();
   }
}

namespace {

   void do_internal_video_mode_uif_irq()
   { 
     
     // typedef video_rows_line_counter sync_timer;
      switch (ivm_mode){
         case video_fields:
            switch(ivm_count){
               case start_of_telemetry_rows:
                //  video_cfg::rows::telem::begin();
                  break;
               case end_of_telemetry_rows:
                 // video_cfg::rows::telem::end();
                  break;
               case start_of_osd_rows:
                 // video_cfg::rows::osd::begin();
                  break;
               case end_of_osd_rows:
                 // video_cfg::rows::osd::end();
                  break;
               case num_video_fields:{
                     if ( first_field == false){ 
                        sync_timer::get()->arr = half_line;
                     }
                     ivm_mode = pre_equalise;
                     ivm_count = 0;
                     return;
                  }
                  break;
               default:
                  break;
            }
            ++ivm_count;
            return;
         case pre_equalise:
            if ( ivm_count == 0){
               sync_timer::get()->ccr1 = short_sync;
               if ( first_field == true){ 
                  sync_timer::get()->arr = half_line;
               }
            }else{
               if ( ivm_count == 5){
                  ivm_mode = vsync;
                  sync_timer::get()->ccr1 = half_line - long_sync;
                  ivm_count = 0;
                  return;
               }
            }
            ++ivm_count;
            return;
         case vsync:
            if (ivm_count == 4){
               ivm_mode = post_equalise;
               sync_timer::get()->ccr1 = short_sync;
//##########################################
             //  first_field = !first_field;
//###########################################
               ivm_count = 0;
               return;
            }
            ++ivm_count;
            return;
         case post_equalise:
            if ( ivm_count == 3) {
               if (first_field == false){
                  sync_timer::get()->arr = full_line;
               }
            }else{
               if (ivm_count == 4){
                  if(first_field == true){
                     sync_timer::get()->arr = full_line;
                  }
                  sync_timer::get()->ccr1 = long_sync;
                  ivm_mode = video_fields;
                  ivm_count = 0;
                  return;
               }
            }
            ++ivm_count;
            return;
         default:
         break;
      }  
   } 

} // namespace

namespace detail{

   void do_internal_video_mode_irq()
   {
      //typedef video_rows_line_counter sync_timer;
      auto const sr_flags = sync_timer::get()->sr.get();
      if ( sr_flags & (1 << 0)) {
         sync_timer::get()->sr.bb_clearbit<0>();
         do_internal_video_mode_uif_irq();
      }
   }

}// detail
