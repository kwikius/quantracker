
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

void Dac_write( uint8_t dacnum, quan::voltage::V const & vout, uint8_t code);

namespace {

   typedef video_rows_line_counter sync_timer;

   void ivm_dac_setup()
   {
// in internal video mode
// only black and white available
// 00 (0) at dac is black_level
// 01 (1) at dac is white
// 10 (20 at dac is sync_tip
     // do need sync_comp may be best to write it now
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

     // also spi clocks only need black
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
         end_of_second_frame,
         pre_equalise,
         vsync,
         post_equalise,
         video_fields
   };

   constexpr uint32_t start_of_telemetry_rows = 3;
   constexpr uint32_t end_of_telemetry_rows = 16;
   constexpr uint32_t start_of_osd_rows = 17;
   constexpr uint32_t end_of_osd_rows = 300;
   constexpr uint32_t num_video_fields = 304;

   ivm_mode_t ivm_mode = video_fields;
   bool first_field = false;
   uint32_t ivm_count = num_video_fields - 1;

} //namespace

namespace detail{

   void sync_sep_setup(osd_state::state_t state);
   void sync_sep_enable();

   void internal_video_mode_setup()
   {
      ivm_mode = video_fields;
      first_field = false;
      ivm_count = num_video_fields - 1;
      
      ivm_dac_setup();
      ivm_pin_setup();
      ivm_timer_setup();
      // set initial timer values
      // get immediate start on overflow
      sync_timer::get()->cnt = 0xffff;
    
      sync_timer::get()->ccr1 = long_sync;

      sync_timer::get()->arr = full_line ;

      NVIC_SetPriority(TIM3_IRQn,interrupt_priority::video);
      NVIC_EnableIRQ (TIM3_IRQn);
      sync_sep_setup(osd_state::internal_video);
      sync_sep_enable();
      quan::stm32::enable<sync_timer>();
   }
}

namespace {

   void do_internal_video_mode_uif_irq()
   { 
      switch (ivm_mode){
         case video_fields:
            ++ivm_count;
            switch( ivm_count){
               case start_of_telemetry_rows:
                  video_cfg::rows::telem::begin();
                  break;
               case end_of_telemetry_rows:
                  video_cfg::rows::telem::end();
                  break;
               case start_of_osd_rows:
                  video_cfg::rows::osd::begin();
                  break;
               case end_of_osd_rows:
                  video_cfg::rows::osd::end();
                  break;
               case num_video_fields:{
                     ivm_count = 0;
                     if ( first_field == false){
                        ivm_mode = end_of_second_frame; 
                     }else{
                        ivm_mode = pre_equalise;
                        sync_timer::get()->ccr1 = short_sync;
                     }
                     sync_timer::get()->arr = half_line;
                  }
                  break;
               default:
                  break;
            }
            break;
         case end_of_second_frame:
            sync_timer::get()->ccr1 = short_sync;
            ivm_mode = pre_equalise;
            break;
         case pre_equalise:
            if ( ++ivm_count == 5){
               ivm_mode = vsync;
               sync_timer::get()->ccr1 = half_line - long_sync;
               ivm_count = 0;
            }
            break;
         case vsync:
            if (++ivm_count == 5){
               ivm_mode = post_equalise;
               sync_timer::get()->ccr1 = short_sync;
               first_field = !first_field;
               ivm_count = 0;
            }
            break;
         case post_equalise:
            if ( first_field){
               if (++ivm_count == 5){
                  ivm_mode = video_fields;
                  sync_timer::get()->ccr1 = long_sync;
                  sync_timer::get()->arr = full_line ;
                  ivm_count = 0;
               }
            }else{
               ++ ivm_count;
               if ( ivm_count == 4){
                  sync_timer::get()->arr = full_line ;
               }else{
                  if (ivm_count == 5){
                     ivm_mode = video_fields;
                     sync_timer::get()->ccr1 = long_sync;
                     ivm_count = 0;
                  }
               }
            }
            break;
         default:
         break;
      }  
   } 

} // namespace

namespace detail{

   void do_internal_video_mode_irq()
   {
      auto const sr_flags = sync_timer::get()->sr.get();
      if ( sr_flags & (1 << 0)) {
         sync_timer::get()->sr.bb_clearbit<0>();
         do_internal_video_mode_uif_irq();
      }
   }

}// detail
