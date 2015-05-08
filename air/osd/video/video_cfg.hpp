#ifndef QUAN_OSD_VIDEO_SETUP_HPP_INCLUDED
#define QUAN_OSD_VIDEO_SETUP_HPP_INCLUDED
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

#include "../resources.hpp"
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/stm32/get_raw_timer_frequency.hpp>
#include <quan/two_d/vect.hpp>

struct video_cfg {

   enum class video_mode_t { unknown,pal,ntsc};

   static video_mode_t get_video_mode(); 

   //row line_counter
   struct rows {
      // TIM3_ETR (hsync) - clock first edge
#if ! defined QUAN_OSD_SOFTWARE_SYNCSEP
      // TIM3_CH1 (vsync) - trigger first edge
#endif
     // typedef quan::stm32::tim3 line_counter;
      typedef video_rows_line_counter line_counter;
      static void setup();
      struct telem {
         static void begin();
         static void end();
         static uint16_t m_begin; // first telemetry row in half_frame
         static uint16_t m_end;    // one beyond last telem row in half_frame
         static uint16_t get_count() // number of telemetry rows in half frame
         {
            return m_end - m_begin;
         }
      };
      struct osd {
         
         static void begin();
         static void end();
         static uint16_t m_begin;
         static uint16_t m_end_pal;
         static uint16_t m_end_ntsc;
         static uint16_t get_end() 
               { return ((get_video_mode() == video_mode_t::pal)?m_end_pal:m_end_ntsc);}
         static uint16_t get_visible_length()
         {
            return get_end() - m_begin;
         }
         uint16_t get_total_length()
         {
            return get_end();
         }
      };
      
      enum class mode {
         idle,telemetry, osd
      };
      static mode get_current_mode() {
         return m_cur_mode;
      }
      static bool is_odd_frame()
      {
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
         return m_cur_row_odd;
#else
         return quan::stm32::get<video_in::odd_even_pin>() == true;
#endif
      }
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
      static void set_odd_frame(){ m_cur_row_odd = true;}
      static void set_even_frame(){ m_cur_row_odd = false;}
#endif     
private:
      static mode m_cur_mode;
#if defined QUAN_OSD_SOFTWARE_SYNCSEP
      static bool m_cur_row_odd;
#endif
   };
   
   struct columns {

      // TIM2_CH1 (hsync)-  trigger 2nd edge ( same on all boards)
      // TIF Interrupt on reinit
      // have time from hsync second edge to line_start to prepare pixel dma
     // typedef quan::stm32::tim2 gate_timer;
      typedef video_columns_gate_timer gate_timer;
      static constexpr quan::frequency::Hz raw_timer_frequency {
            quan::stm32::get_raw_timer_frequency<gate_timer>() 
      };
      static_assert (raw_timer_frequency == quan::frequency::Hz {84000000.0f},"unexpected timer frequency");

      static void setup();
      struct telem{
         static void enable();
         static void disable();
         static void begin();
         static void end();
         static uint16_t m_begin;
         static uint16_t m_end; // one past the end
         static uint16_t get_count()
         {
            return m_end - m_begin;
         }
       };
       struct osd{
         static void enable();
         static void disable();
         static void begin();
         static void end();
         static uint16_t m_begin;
         static uint16_t m_end;
         static uint16_t get_num_pixels()
         {
             return m_end - m_begin;
         }
       };

      static void tif_irq();
      static void uif_irq();
      static void disable();
   };
   
   struct spi_clock {
      // pwm for spi clock
      // TIM1_CH1  except on boadtype 4 which is TIM9_CH1
      // different clk speds for telem and osd
     // typedef quan::stm32::tim1 timer;
      typedef spi_clock_timer timer;
     
      static constexpr quan::frequency::Hz raw_timer_frequency {quan::stm32::get_raw_timer_frequency<timer>() }; 
      static_assert (raw_timer_frequency == quan::frequency::Hz {168000000.0f},"unexpected timer frequency");
      static void setup();
      static uint16_t get_timer_clks_per_px() {
         return m_timer_half_clks_per_px * 2;
      }
      static uint16_t get_telem_clks_per_bit() {
         return m_timer_half_clks_per_bit * 2;
      }
private:
      static uint16_t m_timer_half_clks_per_px;
      static uint16_t m_timer_half_clks_per_bit;
   };
   
   static void setup();

   static quan::two_d::vect<uint32_t> get_display_size_px()
   {
     return {get_display_size_x_px(),get_display_size_y_px()};
   }
   // full visible number of columns
   // adjusted to multiple of 8
   static uint32_t get_display_size_x_px()
   {
      return (columns::osd::get_num_pixels()/8U)*8U;
   }
   // full visible number of rows
   static uint32_t get_display_size_y_px()
   {
#if defined (QUAN_DISPLAY_INTERLACED)
       return rows::osd::get_visible_length();
#else
 return rows::osd::get_visible_length()/2;
#endif
   }
   // dump last pixels if not a multiple of 8
   static uint32_t get_display_size_x_bytes()
   {
       return get_display_size_x_px() / 8U;
   }
#if 0
// not used yet!
   static quan::time::us get_visible_line_period()
   {
       return (spi_clock::get_timer_clks_per_px()
         * get_display_size_x_px())
            / spi_clock::bus_freq;
   }
 #endif  
};

#endif // QUAN_OSD_VIDEO_SETUP_HPP_INCLUDED
