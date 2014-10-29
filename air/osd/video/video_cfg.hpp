#ifndef QUAN_OSD_VIDEO_SETUP_HPP_INCLUDED
#define QUAN_OSD_VIDEO_SETUP_HPP_INCLUDED

#include "../resources.hpp"
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/two_d/vect.hpp>

struct video_cfg {

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
         static uint16_t m_end;
         static uint16_t get_visible_length()
         {
            return m_end - m_begin;
         }
         uint16_t get_total_length()
         {
            return m_end;
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
      // TIM2_CH1 (hsync)-  trigger 2nd edge
      // TIF Interrupt on reinit
      // have time from hsync second edge to line_start to prepare pixel dma
     // typedef quan::stm32::tim2 gate_timer;
      typedef video_columns_gate_timer gate_timer;
      static constexpr quan::frequency::Hz bus_freq {quan::stm32::get_module_bus_frequency<gate_timer>() };
      static_assert (bus_freq == quan::frequency::Hz {42000000.0},"error in bus freq");

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
      // TIM1_CH1 pwm for spi clock
      // different clk speds for telem and osd
     // typedef quan::stm32::tim1 timer;
      typedef spi_clock_timer timer;
     
      static constexpr quan::frequency::Hz bus_freq {quan::stm32::get_module_bus_frequency<timer>() };
      static_assert (bus_freq == quan::frequency::Hz {84000000.0},"error in bus freq");
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
   static quan::time::us get_line_period();
   static void setup();
   
};

#endif // QUAN_OSD_VIDEO_SETUP_HPP_INCLUDED
