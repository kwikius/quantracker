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

#if defined QUAN_OSD_SOFTWARE_SYNCSEP
#include <stm32f4xx.h>
#include <quan/stm32/tim.hpp>
#include <quan/time.hpp>
#include <quan/stm32/get_raw_timer_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include "../resources.hpp"
#include "video_cfg.hpp"
#include "video_buffer.hpp"
#include <quan/uav/osd/api.hpp>

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
   

   synctype_t sync_pulse_type = synctype_t::unknown;
   line_period_t line_period = line_period_t::unknown;
   uint16_t last_sync_first_edge = 0U;
   uint8_t sync_counter = 0U;
   syncmode_t syncmode = syncmode_t::start;
   video_cfg::video_mode_t video_mode = video_cfg::video_mode_t::unknown;

   constexpr uint32_t timer_freq = quan::stm32::get_raw_timer_frequency<sync_sep_timer>();
   constexpr uint16_t clocks_usec =  static_cast<uint16_t>(timer_freq / 1000000U);

   
   static quan::uav::osd::video_mode public_video_mode 
      = quan::uav::osd::video_mode::unknown;

   volatile bool request_suspend_osd_flag = false;
   volatile bool osd_suspended_flag = true;
}; // namespace

void request_osd_suspend()
{
   request_suspend_osd_flag = true;
}

void request_osd_resume()
{
   request_suspend_osd_flag = false;
}

bool osd_suspended()
{
   return osd_suspended_flag;
}

namespace quan{ namespace uav{ namespace osd{

   video_mode get_video_mode()
   {
      return public_video_mode;
   }

}}}

video_cfg::video_mode_t 
video_cfg::get_video_mode()
{
  return video_mode;
}

typedef video_cfg::video_mode_t video_mode_t;
 
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
#if ((QUAN_OSD_BOARD_TYPE != 4) || (defined QUAN_DISCOVERY))
  quan::stm32::set<orange_led_pin>();
 #endif
  initial_first_edge_captured = false;
  sync_pulse_type = synctype_t::unknown;
  line_period = line_period_t::unknown;
  last_sync_first_edge = 0;
  sync_counter = 0;
  syncmode = syncmode_t::start;
  video_mode = video_mode_t::unknown;
}
 
}// namespace 

namespace detail{
void sync_sep_enable()
{
  sync_sep_reset();
  
  sync_sep_timer::get()->sr = 0;
  sync_sep_timer::get()->dier |= (1 << 1) | ( 1 << 2); // ( CC1IE, CC2IE)
  sync_sep_timer::get()->cr1.bb_setbit<0>();// (CEN)
   if ( request_suspend_osd_flag){
     osd_suspended_flag = true;
   }
}
}

namespace {

void sync_sep_disable()
{
  sync_sep_timer::get()->cr1.bb_clearbit<0>() ;// (CEN)
  sync_sep_timer::get()->dier &= ~((1 << 1) | ( 1 << 2)); // ( CC1IE, CC2IE)
}
 
void sync_sep_new_frame()
{
   sync_sep_disable();
   // could we just enable the interrupts
   // Ideally want counting but
   // important if video_mode has changed from ntsc to pal etc
   video_buffers::osd::m_display_size = video_cfg::get_display_size_px();
   video_cfg::rows::line_counter::get()->arr = video_cfg::rows::osd::get_end()/2 - 2;
   // enable the rows counter one shot
   video_cfg::rows::line_counter::get()->cnt = 0;
   // cant see this is ever disabled?
   video_cfg::rows::line_counter::get()->cr1.bb_setbit<0>() ;// CEN
// update the public one.. messy prob just use one public enum for this?
// should prob do it in the swap_buffers function?
   switch (video_mode){
      case video_mode_t::ntsc:
         public_video_mode = quan::uav::osd::video_mode::ntsc;
      break;
      case video_mode_t::pal:
         public_video_mode = quan::uav::osd::video_mode::pal;
      break;
      default:
         public_video_mode = quan::uav::osd::video_mode::unknown;
      break;
   }
}
}
void sync_sep_setup()
{
   quan::stm32::module_enable<video_in_hsync_first_edge_pin::port_type>();
   quan::stm32::module_enable<video_in_hsync_second_edge_pin::port_type>();

   quan::stm32::apply<
      video_in_hsync_first_edge_pin,
// af for first edge
#if (QUAN_OSD_BOARD_TYPE == 4)
      quan::stm32::gpio::mode::af9,  // PB14 TIM12_CH1 
#else
      quan::stm32::gpio::mode::af3,
#endif
#if (QUAN_OSD_BOARD_TYPE == 1) || (QUAN_OSD_BOARD_TYPE == 3) || (QUAN_OSD_BOARD_TYPE == 4)
      quan::stm32::gpio::pupd::pull_up
#else
   #if QUAN_OSD_BOARD_TYPE == 2
      quan::stm32::gpio::pupd::pull_down
   #else
      #error no board defined
   #endif
#endif
   >();

   quan::stm32::apply<
      video_in_hsync_second_edge_pin,
#if (QUAN_OSD_BOARD_TYPE == 4)
      quan::stm32::gpio::mode::af9, // PB15 TIM12_CH2
#else
      quan::stm32::gpio::mode::af3,
#endif
#if (QUAN_OSD_BOARD_TYPE == 1) || (QUAN_OSD_BOARD_TYPE == 3)|| (QUAN_OSD_BOARD_TYPE == 4)
      quan::stm32::gpio::pupd::pull_up
#else
   #if QUAN_OSD_BOARD_TYPE == 2
      quan::stm32::gpio::pupd::pull_down
   #else
      #error no board defined
   #endif
#endif
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
#if (QUAN_OSD_BOARD_TYPE == 1) || (QUAN_OSD_BOARD_TYPE == 3) || (QUAN_OSD_BOARD_TYPE == 4)
      ccer.cc1p = true; // CC1 is falling edge capture
      ccer.cc1np = false;
      ccer.cc2p = false;
      ccer.cc2np = false; // CC2 is rising edge capture
#else
   #if QUAN_OSD_BOARD_TYPE == 2
      ccer.cc1p = false; // CC1 is rising edge capture
      ccer.cc1np = false;
      ccer.cc2p = true;
      ccer.cc2np = false; // CC2 is falling edge capture
   #else
     #error no board type specified
   #endif
#endif
      ccer.cc1e = true;
      ccer.cc2e = true;
    sync_sep_timer::get()->ccer.set(ccer.value);
#if (QUAN_OSD_BOARD_TYPE !=4)
   NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,interrupt_priority::video);
   NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
#else
   NVIC_SetPriority(TIM8_BRK_TIM12_IRQn,interrupt_priority::video);
   NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
#endif
}
 
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
    if (!osd_suspended_flag){
      last_sync_first_edge = sync_sep_timer::get()->ccr1;
      initial_first_edge_captured = true;
    }else{// osd suspended
      if (! request_suspend_osd_flag){
         // want restart
         osd_suspended_flag = false;
      }
    }
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
                        if (  ((video_mode == video_mode_t::pal)  && (sync_counter == 3))
                          ||  ((video_mode == video_mode_t::ntsc) && (sync_counter == 5))
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
                           if ( video_mode != video_mode_t::pal){
                              if ( video_mode == video_mode_t::unknown){
                                 video_mode = video_mode_t::pal;
                              }else{
                                 sync_sep_error_reset(); // unexpected sequence
                                 return;
                              }
                           }
                        }else {
                           if ( sync_counter == 6){
                              if ( video_mode != video_mode_t::ntsc){
                                 if ( video_mode == video_mode_t::unknown){
                                    video_mode = video_mode_t::ntsc;
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
                           if ( video_mode != video_mode_t::pal){
                              if (video_mode == video_mode_t::unknown){
                                 video_mode = video_mode_t::pal;
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
                              if ( video_mode == video_mode_t::pal){
                                  video_cfg::rows::set_odd_frame();
                              }else{
                                 // n.b dont care if its unknown video mode
                                 // assume its ntsc
                                 video_cfg::rows::set_even_frame();
                              }
#endif
                           }else{
                              if ( sync_counter == 6){
                                 if ( video_mode != video_mode_t::ntsc){
                                    if (video_mode == video_mode_t::unknown){
                                       video_mode = video_mode_t::ntsc;
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

#if (QUAN_OSD_BOARD_TYPE == 4)
extern "C" void TIM8_BRK_TIM12_IRQHandler() __attribute__ ( (interrupt ("IRQ")));
extern "C" void TIM8_BRK_TIM12_IRQHandler()
#else
extern "C" void TIM1_BRK_TIM9_IRQHandler() __attribute__ ( (interrupt ("IRQ")));
extern "C" void TIM1_BRK_TIM9_IRQHandler()
#endif
{
     uint16_t const sr = sync_sep_timer::get()->sr.get();
     if (sr & (1 << 1)) {  // cc1_if
          sync_sep_timer::get()->sr.bb_clearbit<1>();
          on_hsync_first_edge();
     } else {
          if (sr & (1 << 2)) {  // cc2_if
            sync_sep_timer::get()->sr.bb_clearbit<2>();
            on_hsync_second_edge();
          }
     }
}

#endif  // defined QUAN_OSD_SOFTWARE_SYNCSEP
