
#include "resources.hpp"
#include <quan/stm32/freertos/millis.hpp>
#include <quan/min.hpp>
#include <quan/uav/osd/basic_font.hpp>
#include <quan/uav/osd/get_aircraft_position.hpp>
#include <quan/uav/osd/features_api.hpp>
#include "on_draw.hpp"
#include "osd.hpp"
#include "symbology.hpp"

using namespace quan::uav::osd;

void draw_gps_state();

void create_mavlink_task();
void create_heartbeat_task();
void create_frsky_task();
void create_fsk_task();
void create_draw_task();

using namespace quan::uav::osd;

namespace {

  enum state_t{init, start, flash_vars, running};

  state_t state = init;

  quan::time_<int64_t>::ms time_now{0LL};

  typedef mavlink_tx_rx_task serial; 

  // position of lower left for pos
  void draw_progress_bar(pxp_type const & pos, pxp_type const & size, uint32_t percent_complete)
  {
     draw_box(pos, pos + size,color_type::black, false);
     pxp_type::value_type const boxlen = ((size.x - 4) * quan::min(percent_complete,100U)) / 100;
     draw_box(pos + pxp_type{2,2} , pos  + pxp_type{boxlen,size.y -2}, color_type::white, true);
  }

  quan::time::ms ready_time{10000};
  void print_ready_message(uint32_t percent)
  {
     font_ptr font = get_font(FontID::OSD_Charset);
     if (font){
        auto display_size = get_display_size();
        auto font_size = get_size(font);
        pxp_type border{5,5};
        pxp_type cursor{
         -display_size.x/2 + border.x,
         display_size.y/2 - (font_size.y * 2   + border.y *2)
        };
        draw_text("Quantracker Air OSD V2.2",cursor,font);
        cursor.y -= font_size.y + border.y ;
        draw_text("Press ret 3 x for flash mod",cursor,font);
        cursor.y -= font_size.y + border.y + 24;
        draw_progress_bar(cursor,pxp_type{display_size.x - 10,20},percent);
     }
  }

   static int count = 0;

   void do_flash_vars()
   {
        font_ptr font = get_font(FontID::OSD_Charset);
        pxp_type border{5,5};
        auto display_size = get_display_size();
        auto font_size = get_size(font);
        pxp_type cursor{
         -display_size.x/2 + border.x,
         0
        };
        draw_text("In Flash Vars ...",cursor,font);
   }
     

   void do_running()
   {
      if ( osd_show_compass() == true){
         draw_compass();
      }
      if (osd_show_afcl_horizon() == true){
         draw_artificial_horizon();
      }
      draw_batteries();
      if (home_position_is_set() == true){
         draw_altitude();
         draw_home();
      } else{ // home position
         if ( position_is_good()){
            if ( ++count == 250){ // 5 secs
               auto const ap = get_aircraft_position();
               set_home_position(ap);
            }
         }else{
            draw_gps_state();
         }
      }
   }
}


void quan::uav::osd::on_draw()
{  
   time_now = quan::stm32::millis();
   switch (state){
      case flash_vars:
         do_flash_vars();
      break;
      case running:
         do_running();
      break;
     case init:
       serial::enable();
       state = start;
       print_ready_message((time_now *100) / ready_time);
     break;
     case start:
       print_ready_message((time_now * 100) / ready_time);
       if (time_now < quan::time::ms{200} ){
          // flush any startup junk
          while( serial::in_avail()){
            serial::get();
          } 
          break;
       }else{
         if (time_now < ready_time ){
            if (serial::in_avail()){
               char ch = serial::get();
               if (( ch == '\r') || (ch == '\n')){
                   if (++count == 3){
                        state = flash_vars;
                   }
               }else{ // not a ret
                   state = running;
               }
            }
         }else{ // out of time
              state = running;
         }
         if (state == running){
           create_mavlink_task();
           create_frsky_task();
           create_heartbeat_task();
           create_fsk_task();
           create_draw_task();
           count = 0;
         }
      }
      break;
      default:
      break;
   }
}