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
#include <quan/stm32/tim.hpp>
#include <quan/time.hpp>
#include <quan/stm32/get_raw_timer_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include "../resources.hpp"
#include "video_cfg.hpp"
#include "video_buffer.hpp"
#include "osd_state.hpp"
// for std::abs
#include <cstdlib>

#include <quan/uav/osd/api.hpp>

#if 1
namespace {

   bool initial_first_edge_captured = false;
   uint16_t last_sync_first_edge = 0U;
   int32_t vsync_count = -1;
    //for  use in the irq
   quan::uav::osd::video_mode private_video_mode = quan::uav::osd::video_mode::unknown;
   // the public one updated when the vsync is parsed successfully
   quan::uav::osd::video_mode public_video_mode = quan::uav::osd::video_mode::unknown;

   constexpr uint32_t timer_freq_Hz = quan::stm32::get_raw_timer_frequency<sync_sep_timer>();
   constexpr uint32_t  timer_freq_MHz = timer_freq_Hz / 1000000U;
   static_assert(timer_freq_MHz == 84U,"unexpected timer freq,  need to redo timings");
   // 2.35 usec in timer_clks
   constexpr uint16_t vsync_p_clks  = (timer_freq_MHz * 235 ) / 100
         + ((((timer_freq_MHz * 235 ) % 100 ) >= 50)?1:0);
   static_assert(vsync_p_clks == 197U,"unexpected timer freq,  need to redo timings");
   // 64 usec H (full line period)  in timer clks
   constexpr uint16_t vsync_H_clks = timer_freq_MHz * 64U;
   // gives 0.5 usec each side
   constexpr uint16_t delta_H_clks = 42U;
   // add this value to nominal to give a +- tolerance on the p sync pulse
   // 21 value gives 0.25 usec each side
   constexpr uint16_t delta_p_clks = 21U;
 
   // 4.7 usec normal hsync pulse length
   constexpr uint16_t vsync_r_clks  = (timer_freq_MHz * 47 ) / 10
         + ((((timer_freq_MHz * 47 ) % 10 ) >= 5)?1:0);

   static_assert(vsync_r_clks == 395U,"unexpected timer freq,  need to redo timings");
   constexpr uint16_t delta_r_clks = 21U;

   constexpr uint16_t vsync_q_clks = vsync_H_clks/2 - vsync_r_clks;

   constexpr uint16_t delta_q_clks = 42U;

   // various modes for parsing sync pulse
   // A grammar might be a thoughT
   enum class syncmode_t {
      seeking,
      pre_equalise,
      vsync_serration,
      post_equalise
   };

   enum class sync_pulse_t { unknown, out_of_range, hsync, equalising, field_sync};
   enum class line_period_t {unknown, out_of_range, full, half};

   syncmode_t syncmode = syncmode_t::seeking;
   sync_pulse_t last_sync_pulse = sync_pulse_t::unknown;
   line_period_t last_line_period = line_period_t::unknown;

   void sync_sep_reset() 
   {
      initial_first_edge_captured = false;
      syncmode = syncmode_t::seeking;
      last_sync_pulse = sync_pulse_t::unknown;
      last_line_period = line_period_t::unknown;
      vsync_count = -1;
   }

   // returns true if not out of range
   // latest first edge in sync_sep_timer::get()->ccr1;
   // only called if last_sync_first_edge is valid
    // ( initial_first_edge_captured == true)
   bool calc_line_period( )
   {
      // either full or half line period is valid
      uint16_t const capture = sync_sep_timer::get()->ccr1;
      uint16_t const period = capture - last_sync_first_edge;
      last_sync_first_edge = capture;
      if ( period  <= ( (vsync_H_clks + delta_H_clks)/2) ){
         if ( period >= ( (vsync_H_clks - delta_H_clks)/2) ){
            last_line_period = line_period_t::half;
            return true;
         }
      }else{
         if ( period >= ( vsync_H_clks - delta_H_clks) ){
            if (period  <= ( vsync_H_clks + delta_H_clks) ){
               last_line_period = line_period_t::full;
               return true;
            }
         }
      }
      
      last_line_period = line_period_t::out_of_range;
      return false;
   }

   // returns true if not out of range
   // requires a valid last_sync_first_edge
   int32_t pulse_error_count = 0;
   bool calc_sync_pulse_type()
   {
      uint16_t const capture = sync_sep_timer::get()->ccr2;
      uint16_t const pulselen = capture - last_sync_first_edge;
      if ( pulselen <= ( vsync_p_clks + delta_p_clks)){
         if ( pulselen >= ( vsync_p_clks - delta_p_clks)){
            last_sync_pulse = sync_pulse_t::equalising;
            return true;
         }
      }else{
         if(pulselen >= (vsync_q_clks - delta_q_clks) ){
            if ( pulselen <= (vsync_q_clks + delta_q_clks) ){
               last_sync_pulse = sync_pulse_t::field_sync;
               return true;
            }
         }else{
            if ( (pulselen >= ( vsync_r_clks - delta_r_clks)) && 
                 (pulselen <= ( vsync_r_clks + delta_r_clks))
               ){
               last_sync_pulse = sync_pulse_t::hsync;
               return true;
            }
         }
      }
     
      last_sync_pulse = sync_pulse_t::out_of_range;
      return false;
   }

   // parse the current vsync event
   // called from first edge after processing last line period
   void sync_filter()
   {
     
      switch (syncmode){
         case syncmode_t::seeking:
            
            if ( last_line_period == line_period_t::half){
               if (last_sync_pulse == sync_pulse_t::equalising){
                  // first preequalising pulse
                  syncmode = syncmode_t::pre_equalise;
                  vsync_count = 1;
                  return;
               }
            }
            
            sync_sep_reset();
            return;
         case syncmode_t::pre_equalise:
            if ( last_line_period == line_period_t::half){
               if (last_sync_pulse == sync_pulse_t::field_sync){
                  if ( vsync_count >= 4){
                     syncmode = syncmode_t::vsync_serration;
                     vsync_count = 1;
                     return;
                  }
               }else {
                  if ( last_sync_pulse == sync_pulse_t::equalising){
                     if ( ++vsync_count < 7){
                        return;
                     }
                  }
               }
               
            }
            sync_sep_reset();
            return;
         case syncmode_t::vsync_serration:
            if ( last_line_period == line_period_t::half){
               if ( last_sync_pulse == sync_pulse_t::equalising){
                  if (vsync_count == 6){
                     private_video_mode = quan::uav::osd::video_mode::ntsc;
                     syncmode = syncmode_t::post_equalise;
                     vsync_count = 1;
                     return;
                  }else{
                     if (vsync_count == 5){
                        private_video_mode = quan::uav::osd::video_mode::pal;
                        syncmode = syncmode_t::post_equalise;
                        vsync_count = 1;
                        return;
                     }
                  }
               }else{
                  if( last_sync_pulse == sync_pulse_t::field_sync){
                      if (++vsync_count < 7){
                        return;
                      }
                  }
               }
            }
        //    quan::stm32::set<heartbeat_led_pin>();
            sync_sep_reset();
            return;
         case syncmode_t::post_equalise:
            if( last_sync_pulse == sync_pulse_t::equalising){
               ++ vsync_count;
               if ( private_video_mode == quan::uav::osd::video_mode::ntsc){
                  if ( vsync_count < 7){
                     return;
                  }
               }else{
                  if ( private_video_mode == quan::uav::osd::video_mode::pal){
                     if ( vsync_count < 6){
                        return;
                     }
                  }
               }
            }
        //    quan::stm32::set<heartbeat_led_pin>();
            sync_sep_reset();
            return;
      }
   }
   
   void on_hsync_first_edge()
   { 
      if (initial_first_edge_captured){
         if (!calc_line_period()){
            sync_sep_reset();
            return;
         }else{
            sync_filter();   
         }
      }else{
         last_sync_first_edge = sync_sep_timer::get()->ccr1;
         initial_first_edge_captured = true;
      }
   }

   void sync_sep_disable()
   {
     sync_sep_timer::get()->cr1.bb_clearbit<0>() ;// (CEN)
     sync_sep_timer::get()->dier &= ~((1 << 1) | ( 1 << 2)); // ( CC1IE, CC2IE)
   }

// signal at this point there is external video
   void sync_sep_new_frame()
   {
      if( osd_state::get() == osd_state::external_video){
        // quan::stm32::clear<heartbeat_led_pin>();
         sync_sep_disable();
         public_video_mode = private_video_mode;
         // important if video_mode has changed from ntsc to pal etc
         video_buffers::osd::m_display_size = video_cfg::get_display_size_px();
         video_cfg::rows::line_counter::get()->arr = video_cfg::rows::osd::get_end()/2 - 2;
         // enable the rows counter one shot
         video_cfg::rows::line_counter::get()->cnt = 0;
         video_cfg::rows::line_counter::get()->cr1.bb_setbit<0>() ;// CEN
      }else{
          osd_state::set_have_external_video();
          sync_sep_reset();
      }
   }


   // and for hsyn hsync pulse
   // then syncmode == post_equalising
   // and if PAL ( count == 5)
   // if NTSC count == 6
   // then if period == full then odd even etc
   // for valid new frame
   void on_hsync_second_edge() 
   {
      if ( initial_first_edge_captured){
         if (calc_sync_pulse_type()){
            if (last_sync_pulse == sync_pulse_t::hsync){
               if ( syncmode == syncmode_t::post_equalise){
                  if ( private_video_mode == quan::uav::osd::video_mode::ntsc){
                     if ( vsync_count == 6){
                     // do odd or even depending on length of period half or full
                        sync_sep_new_frame();
                     }else{
                   //     quan::stm32::set<heartbeat_led_pin>();
                        sync_sep_reset();
                     }
                  }else{
                     if ( private_video_mode == quan::uav::osd::video_mode::pal){
                        if ( (vsync_count == 4) ||  (vsync_count == 5)){
                          // do odd or even depending on length of period half or full
                           sync_sep_new_frame();
                        }else{
                           quan::stm32::set<heartbeat_led_pin>();
                           sync_sep_reset();
                        }
                     }
                  }   
               }else{ // hsync &&  not postequalise
 //                 quan::stm32::set<heartbeat_led_pin>();
                  sync_sep_reset();
               }
               return;
            }else{ // not hsync so continue getting the vsync pulse train
               return;
            }
         }else{ // failed to calc sync pulse type
 //           quan::stm32::set<heartbeat_led_pin>();
            sync_sep_reset();
            return;
         }
      }else{ // no initial first edeg so ignore
         return;
      }
   }
}

namespace quan{ namespace uav{ namespace osd{

   video_mode get_video_mode()
   {  if( osd_state::get() == osd_state::external_video){
         return public_video_mode;
      }else{
         return quan::uav::osd::video_mode::pal;
      }
   }

}}}


namespace detail{

   void sync_sep_enable()
   {
     sync_sep_reset();
     sync_sep_timer::get()->sr = 0;
     sync_sep_timer::get()->dier |= (1 << 1) | ( 1 << 2); // ( CC1IE, CC2IE)
     sync_sep_timer::get()->cr1.bb_setbit<0>();// (CEN)
      
   }

   void sync_sep_takedown()
   {
      NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);
     // sync_sep_osd_state = osd_state::suspended;
      private_video_mode = quan::uav::osd::video_mode::unknown;
      // public_video_mode ?
      quan::stm32::apply<
         video_in_hsync_first_edge_pin,
         quan::stm32::gpio::mode::input,  // PB14 TIM12_CH1    // af for first edge
         quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::apply<
         video_in_hsync_second_edge_pin,
         quan::stm32::gpio::mode::input, // PB15 TIM12_CH2
         quan::stm32::gpio::pupd::pull_up
      >();
   }

   void sync_sep_setup()
   {
      
      quan::stm32::module_enable<video_in_hsync_first_edge_pin::port_type>();
      quan::stm32::module_enable<video_in_hsync_second_edge_pin::port_type>();

      quan::stm32::apply<
         video_in_hsync_first_edge_pin,
         quan::stm32::gpio::mode::af9,  // PB14 TIM12_CH1    // af for first edge
         quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::apply<
         video_in_hsync_second_edge_pin,
         quan::stm32::gpio::mode::af9, // PB15 TIM12_CH2
         quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::module_enable<sync_sep_timer>();
      quan::stm32::module_reset<sync_sep_timer>();

      sync_sep_timer::get()->arr = 0xFFFF;

      // cc1 capture on first edge of hsync
      // cc2 capture on second edge of hsync
      quan::stm32::tim::ccmr1_t ccmr1 = 0;
      ccmr1.cc1s = 0b01;// CC1 is input mapped on TI1
      ccmr1.cc2s = 0b01; // CC2 is input mapped on TI2
      sync_sep_timer::get()->ccmr1.set(ccmr1.value);
      quan::stm32::tim::ccer_t ccer = 0;
      ccer.cc1p = true; // CC1 is falling edge capture
      ccer.cc1np = false;
      ccer.cc2p = false;
      ccer.cc2np = false; // CC2 is rising edge capture
      ccer.cc1e = true;
      ccer.cc2e = true;
      sync_sep_timer::get()->ccer.set(ccer.value);
      switch (osd_state::get()){
         case osd_state::external_video:
            NVIC_SetPriority(TIM8_BRK_TIM12_IRQn,interrupt_priority::video);
            NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
         break;
         case osd_state::internal_video:
            NVIC_SetPriority(TIM8_BRK_TIM12_IRQn,interrupt_priority::fsk_dac_timer);
            NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
         break;
         default:
         break;
      }
   }

} //detail

#else

//######################################################################################


/*
software sync sep
Use with telem only
or use with OSD but dont use last 2 lines of visible display.
so leaving 2 frames to catch preequalise run in for vsync.
vsync disables the syn sep routine and starts the row counter for telem and osd irqs 
end of telem and osd re-enables the routine to pickup next vsync and odd even frame
Uses a separate capture for first and second edge then timing can be quite relaxed
Both are in same irq so low edge can be dealt with before high edge
TODO add ADC to get sync tip and black level
*/

namespace {

   bool initial_first_edge_captured = false;
    
   enum class synctype_t {
        unknown,hsync, vsync
   };
   enum class line_period_t {
        unknown, half, full
   };
   enum class syncmode_t {
        start,
        sync_phase1,
        pre_equalise,
        vsync_serration,
        post_equalise
   };
   
  // osd_state::state_t sync_sep_osd_state = osd_state::suspended;
   synctype_t sync_pulse_type = synctype_t::unknown;
   line_period_t line_period = line_period_t::unknown;
   uint16_t last_sync_first_edge = 0U;
   uint8_t sync_counter = 0U;
   syncmode_t syncmode = syncmode_t::start;

   constexpr uint32_t timer_freq = quan::stm32::get_raw_timer_frequency<sync_sep_timer>();
   constexpr uint16_t clocks_usec =  static_cast<uint16_t>(timer_freq / 1000000U);

// change video_mode to next_frame_video mode
   quan::uav::osd::video_mode video_mode = quan::uav::osd::video_mode::unknown;
   // video_mode relates to pal ntsc
   quan::uav::osd::video_mode public_video_mode 
      = quan::uav::osd::video_mode::unknown;

}; // namespace



namespace quan{ namespace uav{ namespace osd{

   video_mode get_video_mode()
   {  if( osd_state::get() == osd_state::external_video){
         return public_video_mode;
      }else{
         return quan::uav::osd::video_mode::pal;
      }
   }

}}}

namespace {
   void sync_sep_reset()
   {
     initial_first_edge_captured = false;
     sync_pulse_type = synctype_t::unknown;
     line_period = line_period_t::unknown;
     last_sync_first_edge = 0U;
     sync_counter = 0U;
     syncmode = syncmode_t::start;
   }

   void sync_sep_error_reset()
   {
     sync_sep_reset();
     video_mode = quan::uav::osd::video_mode::unknown;
   }

   
}// namespace 

namespace detail{
   void sync_sep_enable()
   {
     sync_sep_reset();
     sync_sep_timer::get()->sr = 0;
     sync_sep_timer::get()->dier |= (1 << 1) | ( 1 << 2); // ( CC1IE, CC2IE)
     sync_sep_timer::get()->cr1.bb_setbit<0>();// (CEN)
      
   }

   void sync_sep_takedown()
   {
      NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);
     // sync_sep_osd_state = osd_state::suspended;
      video_mode = quan::uav::osd::video_mode::unknown;
      quan::stm32::apply<
         video_in_hsync_first_edge_pin,
         quan::stm32::gpio::mode::input,  // PB14 TIM12_CH1    // af for first edge
         quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::apply<
         video_in_hsync_second_edge_pin,
         quan::stm32::gpio::mode::input, // PB15 TIM12_CH2
         quan::stm32::gpio::pupd::pull_up
      >();
   }
} //detail

namespace {

   void sync_sep_disable()
   {
     sync_sep_timer::get()->cr1.bb_clearbit<0>() ;// (CEN)
     sync_sep_timer::get()->dier &= ~((1 << 1) | ( 1 << 2)); // ( CC1IE, CC2IE)
   }
    
   // signal at this point there is external video
   void sync_sep_new_frame()
   {
      if( osd_state::get() == osd_state::external_video){
        // quan::stm32::clear<heartbeat_led_pin>();
         sync_sep_disable();
         public_video_mode = video_mode;
         // important if video_mode has changed from ntsc to pal etc
         video_buffers::osd::m_display_size = video_cfg::get_display_size_px();
         video_cfg::rows::line_counter::get()->arr = video_cfg::rows::osd::get_end()/2 - 2;
         // enable the rows counter one shot
         video_cfg::rows::line_counter::get()->cnt = 0;
         video_cfg::rows::line_counter::get()->cr1.bb_setbit<0>() ;// CEN
      }else{
          osd_state::set_have_external_video();
          sync_sep_reset();
      }
   }
} // namespace

namespace detail{

   void sync_sep_setup()
   {
      
      quan::stm32::module_enable<video_in_hsync_first_edge_pin::port_type>();
      quan::stm32::module_enable<video_in_hsync_second_edge_pin::port_type>();

      quan::stm32::apply<
         video_in_hsync_first_edge_pin,
         quan::stm32::gpio::mode::af9,  // PB14 TIM12_CH1    // af for first edge
         quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::apply<
         video_in_hsync_second_edge_pin,
         quan::stm32::gpio::mode::af9, // PB15 TIM12_CH2
         quan::stm32::gpio::pupd::pull_up
      >();

      quan::stm32::module_enable<sync_sep_timer>();
      quan::stm32::module_reset<sync_sep_timer>();

      sync_sep_timer::get()->arr = 0xFFFF;

      // cc1 capture on first edge of hsync
      // cc2 capture on second edge of hsync
      quan::stm32::tim::ccmr1_t ccmr1 = 0;
      ccmr1.cc1s = 0b01;// CC1 is input mapped on TI1
      ccmr1.cc2s = 0b01; // CC2 is input mapped on TI2
      sync_sep_timer::get()->ccmr1.set(ccmr1.value);
      quan::stm32::tim::ccer_t ccer = 0;
      ccer.cc1p = true; // CC1 is falling edge capture
      ccer.cc1np = false;
      ccer.cc2p = false;
      ccer.cc2np = false; // CC2 is rising edge capture
      ccer.cc1e = true;
      ccer.cc2e = true;
      sync_sep_timer::get()->ccer.set(ccer.value);
      switch (osd_state::get()){
         case osd_state::external_video:
            NVIC_SetPriority(TIM8_BRK_TIM12_IRQn,interrupt_priority::video);
            NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
         break;
         case osd_state::internal_video:
            NVIC_SetPriority(TIM8_BRK_TIM12_IRQn,interrupt_priority::fsk_dac_timer);
            NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
         break;
         default:
         break;
      }
   }
}// detail
 
namespace {
// on first edge
// measure time from last first_edge
void  calc_line_period() 
{
  if (initial_first_edge_captured) {
       uint16_t const capture = sync_sep_timer::get()->ccr1;
       uint16_t const line_length = capture - last_sync_first_edge ;
       last_sync_first_edge = capture;
       constexpr uint16_t one_quarter_line = 16U * clocks_usec;
       constexpr uint16_t three_quarter_line = 48U * clocks_usec;
       constexpr uint16_t one_and_a_quarter_line = 80U * clocks_usec;
       if ( (line_length > one_quarter_line) && (line_length < three_quarter_line)) {
            line_period = line_period_t::half;
       }else{
         if ( (line_length < one_and_a_quarter_line) && (line_length >= three_quarter_line)) {
              line_period = line_period_t::full;
         } else {
              sync_sep_reset();
         }
       }
  } else {// just get initial capture value and exit
      last_sync_first_edge = sync_sep_timer::get()->ccr1;
      initial_first_edge_captured = true;
  } 
}
 
// on second edge
// measure how long low pulse was
// and decide if it's a hsync, vsync or invalid

void calc_sync_pulse_type() 
{
   if ( initial_first_edge_captured) { // have capture to compare with
      // second edge capture
      uint16_t const capture = sync_sep_timer::get()->ccr2;
      uint16_t const sync_length = capture - last_sync_first_edge ;
      // hsync short is about 2.4 usec
      // hsync long is about 4.8 usec
      // dont diff between the 2 here
      constexpr uint16_t min_hsync_len = 1U * clocks_usec; 
      constexpr uint16_t max_hsync_len = 10U * clocks_usec; 
      // vsync fall to rise is about 27.3 usec
      constexpr uint16_t min_vsync_len = 20U * clocks_usec; 
      constexpr uint16_t max_vsync_len = 35U * clocks_usec; 
      if ( (sync_length >= min_vsync_len) && (sync_length <= max_vsync_len) ) {
               sync_pulse_type = synctype_t::vsync;
      }else {
         if ( (sync_length <= max_hsync_len) && (sync_length >= min_hsync_len)) {
               sync_pulse_type = synctype_t::hsync;
         }else {
            sync_sep_reset();
         }
      }
   }
}
 
// TDO vsync ADC for low value here
// in vsync
void on_hsync_first_edge()
{ 
    calc_line_period();
}

void on_hsync_second_edge() 
{
     calc_sync_pulse_type(); 
     if ( (sync_pulse_type != synctype_t::unknown)
               && (line_period != line_period_t::unknown) ) {
          switch (syncmode) {
                case syncmode_t::post_equalise:
// here it should always be known whether in pal or ntsc
                  if ( (line_period == line_period_t::half) 
                           && (sync_pulse_type == synctype_t::hsync) ) {
                        ++sync_counter;
#if 0
                        if (sync_counter == 1){
                              // TODO Do ADC to get black _level
                        }
                        if (sync_counter == 2){
                              // TODO get ADC result and convert sort outputs etc
                        }
#endif
                        if (  ((video_mode == quan::uav::osd::video_mode::pal)  && (sync_counter == 3))
                          ||  ((video_mode == quan::uav::osd::video_mode::ntsc) && (sync_counter == 5))
                        ){

                           // disable this sequence and start
                           // osd and telem sequence
                           sync_sep_new_frame();               
                        }
                  }else{ 
                     sync_sep_error_reset(); // unexpected pulse type or frame length
                  }
                break;
                case syncmode_t::vsync_serration:
                  if (line_period == line_period_t::half){
                     if (sync_pulse_type == synctype_t::hsync){
                     // signifies the end of vsync period
                        // TODO flag calc_line_period to start ADC conv for sync tip
                        if ( sync_counter == 5){
                           
                           if ( video_mode != quan::uav::osd::video_mode::pal){
                              if ( video_mode == quan::uav::osd::video_mode::unknown){
                                 video_mode = quan::uav::osd::video_mode::pal;
                              }else{
                                 sync_sep_error_reset(); // unexpected sequence
                                 return;
                              }
                           }
                        }else {
                           if ( sync_counter == 6){
                              if ( video_mode != quan::uav::osd::video_mode::ntsc){
                                 if ( video_mode == quan::uav::osd::video_mode::unknown){
                                    video_mode = quan::uav::osd::video_mode::ntsc;
                                 }else{
                                    sync_sep_error_reset(); // unexpected sequence
                                    return;
                                 }
                              }
                           }else{
                              sync_sep_error_reset(); // unexpected sequence
                              return;
                           }
                        }
                        // TODO get sync tip ADC result
                        syncmode = syncmode_t::post_equalise;
                        sync_counter = 0 ;
                     }else{// incr count in the vsync period
                        if (++sync_counter > 8){
                           sync_sep_error_reset(); // unexpected sequence
                        }
                     }
                  }else{ // not a half line period in vsync
                     sync_sep_error_reset(); // unexpected
                  }
                break;
                case syncmode_t::pre_equalise:
                  if (line_period == line_period_t::half) {
                       if (sync_pulse_type == synctype_t::vsync) {
                        // start of vsync period
                        // do odd / even frame logic
                        // n.b not realy needed in non-interlaced
                        if (sync_counter == 4){
                           if ( video_mode != quan::uav::osd::video_mode::pal){
                              if (video_mode == quan::uav::osd::video_mode::unknown){
                                 video_mode = quan::uav::osd::video_mode::pal;
                              }else{
                                 sync_sep_error_reset(); // unexpected
                                 return;
                              }
                           }
#if defined (QUAN_DISPLAY_INTERLACED)
                           video_cfg::rows::set_even_frame();
#endif
                        }else {
                           if ( sync_counter == 5){
#if defined (QUAN_DISPLAY_INTERLACED)
                              if ( video_mode == quan::uav::osd::video_mode::pal){
                                  video_cfg::rows::set_odd_frame();
                              }else{
                                 // n.b dont care if its unknown video mode
                                 // assume its ntsc
                                 video_cfg::rows::set_even_frame();
                              }
#endif
                           }else{
                              if ( sync_counter == 6){
                                 if ( video_mode != quan::uav::osd::video_mode::ntsc){
                                    if (video_mode == quan::uav::osd::video_mode::unknown){
                                       video_mode = quan::uav::osd::video_mode::ntsc;
                                    }else{
                                       sync_sep_error_reset(); // unexpected
                                       return;
                                    }
                                 }
#if defined (QUAN_DISPLAY_INTERLACED)
                                 video_cfg::rows::set_odd_frame();
#endif
                              }else{ // invalid number of pre-equalise pulses
                                 sync_sep_error_reset(); // unexpected
                                 return;
                              }
                           }
                        } // ~ synccounter 4 to 6
                        syncmode = syncmode_t::vsync_serration;
                        sync_counter = 1;
                     }else { // hsync pulse in pre_equalise
                        if (++sync_counter > 6) {
                           sync_sep_error_reset();
                        }
                     }
                  }else{ // unexpected full line period
                     sync_sep_error_reset(); 
                  }
               break;
               case syncmode_t::start:
                  if ( (sync_pulse_type == synctype_t::hsync)
                            && (line_period == line_period_t::full)) {
                       //in frame so move to next state
                       syncmode = syncmode_t::sync_phase1;
                  }
               break;
               case syncmode_t::sync_phase1:
                  if (sync_pulse_type == synctype_t::hsync) {
                    if (line_period == line_period_t::half) {
                         // transition to pre_equalise
                         // for odd lines the first half line is last picture line
                         // rather than preequalise but this is counted
                         syncmode = syncmode_t::pre_equalise;
                         sync_counter = 1;
                    }
                  } else {
                    sync_sep_error_reset(); // unexpected vsync
                  }
               break;
               default:
               break;
          }
     }
}
} // namespace

// old sync_sep routine
//########################################################################
#endif

/* 
   todo add test for overcapture ?
*/
extern "C" void TIM8_BRK_TIM12_IRQHandler() __attribute__ ( (interrupt ("IRQ")));
extern "C" void TIM8_BRK_TIM12_IRQHandler()
{
   uint16_t const sr = sync_sep_timer::get()->sr.get();
  
   if (sr & (1 << 1)) {  // cc1_if
      sync_sep_timer::get()->sr.bb_clearbit<1>();
      on_hsync_first_edge();
   }
   if (sr & (1 << 2)) {  // cc2_if
      sync_sep_timer::get()->sr.bb_clearbit<2>();
      on_hsync_second_edge();
   } 
}

