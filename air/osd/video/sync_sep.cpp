
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
#include <stm32f4xx.h>
#include <quan/stm32/tim.hpp>
#include <quan/time.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include "../resources.hpp"
#include "video_cfg.hpp"

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
  // typedef quan::stm32::tim9 sync_timer;
    
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

   constexpr uint32_t bus_freq = quan::stm32::get_module_bus_frequency<sync_sep_timer>();
   constexpr uint16_t clocks_usec = 2U * static_cast<uint16_t>(bus_freq / 1000000U);

}; // namespace
 
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
  quan::stm32::set<blue_led_pin>();
  initial_first_edge_captured = false;
  sync_pulse_type = synctype_t::unknown;
  line_period = line_period_t::unknown;
  last_sync_first_edge = 0;
  sync_counter = 0;
  syncmode = syncmode_t::start;
}

void sync_sep_enable()
{
  sync_sep_reset();
  sync_sep_timer::get()->sr = 0;
  sync_sep_timer::get()->dier |= (1 << 1) | ( 1 << 2); // ( CC1IE, CC2IE)
  sync_sep_timer::get()->cr1.bb_setbit<0>();// (CEN)
}

void sync_sep_disable()
{
  sync_sep_timer::get()->cr1.bb_clearbit<0>() ;// (CEN)
  sync_sep_timer::get()->dier &= ~((1 << 1) | ( 1 << 2)); // ( CC1IE, CC2IE)
}
 
void sync_sep_new_frame()
{
  sync_sep_disable();
  // enable the rows counter one shot
  video_cfg::rows::line_counter::get()->cnt = 0;
  video_cfg::rows::line_counter::get()->cr1.bb_setbit<0>() ;// CEN
  
}

void sync_sep_setup()
{
   quan::stm32::module_enable<video_in_hsync_first_edge_pin::port_type>();
   quan::stm32::module_enable<video_in_hsync_second_edge_pin::port_type>();
//############################
//change to pulldown
   quan::stm32::apply<
      video_in_hsync_first_edge_pin,
      quan::stm32::gpio::mode::af3,
#if (QUAN_OSD_BOARD_TYPE == 1) || (QUAN_OSD_BOARD_TYPE == 3)
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
      quan::stm32::gpio::mode::af3,
#if (QUAN_OSD_BOARD_TYPE == 1) || (QUAN_OSD_BOARD_TYPE == 3)
      quan::stm32::gpio::pupd::pull_up
#else
   #if QUAN_OSD_BOARD_TYPE == 2
      quan::stm32::gpio::pupd::pull_down
   #else
      #error no board defined
   #endif
#endif
   >();
//####################################

   quan::stm32::module_enable<sync_sep_timer>();
   quan::stm32::module_reset<sync_sep_timer>();

   sync_sep_timer::get()->arr = 0xFFFF;

   // cc1 capture on first edge of hsync
   // cc2 capture on second edge of hsync
   quan::stm32::tim::ccmr1_t ccmr1 = 0;
     ccmr1.cc1s = 0b01;// CC1 is input mapped on TI1
     ccmr1.cc2s = 0b01; // CC2 is input mapped on TI2
   sync_sep_timer::get()->ccmr1.set(ccmr1.value);

// tim9
   quan::stm32::tim::ccer_t ccer = 0;
#if (QUAN_OSD_BOARD_TYPE == 1) || (QUAN_OSD_BOARD_TYPE == 3)
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

   NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,interrupt_priority::video);
   NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
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
       } else {
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
             //  quan::stm32::set<red_led_pin>();
               //quan::stm32::set<test_pin>();
      }else {
             // quan::stm32::clear<test_pin>();
         if ( (sync_length <= max_hsync_len) && (sync_length >= min_hsync_len)) {
               sync_pulse_type = synctype_t::hsync;
              // quan::stm32::clear<orange_led_pin>();
         }else {
            sync_sep_reset();
              if(sync_length >= max_hsync_len){
            // quan::stm32::set<orange_led_pin>();
             }
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
   //  quan::stm32::clear<test_pin>();
     calc_sync_pulse_type(); 
     if ( (sync_pulse_type != synctype_t::unknown)
               && (line_period != line_period_t::unknown) ) {
            
          switch (syncmode) {
                case syncmode_t::post_equalise:
                  if ( (line_period == line_period_t::half) 
                           && (sync_pulse_type == synctype_t::hsync) ) {
                        ++sync_counter;
                        if (sync_counter == 1){
                              // Do ADC to get black _level
                        }
                        if (sync_counter ==2){
                              // get ADC result and convert sort outputs etc
                        }
                        if (sync_counter == 3){
                           // dont disable but
                           sync_sep_new_frame(); // disable this sequence and start
                                             // osd and telem sequence
                        }
                  }else{ 
                     sync_sep_error_reset(); // unexpected puls type or frame length
                  }
                break;
                case syncmode_t::vsync_serration:
                  if (line_period == line_period_t::half){
                     if (sync_pulse_type == synctype_t::hsync){
                        
                        if ( sync_counter == 4){
                             // flag calc_line_period to start ADC conv for sync tip
                           //quan::stm32::clear<test_output_pin>();
                        }
                        if ( sync_counter == 5){
                           // get sync tip ADC result
                           syncmode = syncmode_t::post_equalise;
                           sync_counter = 0 ;
                        }else{
                        //   quan::stm32::set<red_led_pin>();
                           sync_sep_error_reset(); // unexpected sequence
                        }
                     }else{
                        if (++sync_counter > 7){
                     //      quan::stm32::set<green_led_pin>();
                           sync_sep_error_reset(); // unexpected sequence
                        }
                     }
                  }else{
                     sync_sep_error_reset(); // unexpected
               //      quan::stm32::set<orange_led_pin>();
                  }
                break;
                case syncmode_t::pre_equalise:
                  if (line_period == line_period_t::half) {
                       if (sync_pulse_type == synctype_t::vsync) {
                           quan::stm32::complement<test_output_pin>(); 
                            if (sync_counter == 5){
                                 video_cfg::rows::set_odd_frame();
                            }else{
                              if(sync_counter == 4){
                                 video_cfg::rows::set_even_frame();
                              }else{
                                 sync_sep_error_reset(); // unexpected
                                 return;
                              }
                            }
                            syncmode = syncmode_t::vsync_serration;
                            
                            sync_counter = 1;
                       }else { // hsync pulse
                           if (++sync_counter > 5) {
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
                          //   quan::stm32::clear<green_led_pin>();
                          //   quan::stm32::set<red_led_pin>();
                       }else{
                          //     quan::stm32::clear<red_led_pin>();
                          //    quan::stm32::set<green_led_pin>();
                        }
                  } else {
                       sync_sep_error_reset(); // unexpected vsync
                  }
               break;
               default:
               break;
          }
     }else{
         
     }
}
} // namespace
 
extern "C" void TIM1_BRK_TIM9_IRQHandler() __attribute__ ( (interrupt ("IRQ")));
 
extern "C" void TIM1_BRK_TIM9_IRQHandler()
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
 
 
 
 