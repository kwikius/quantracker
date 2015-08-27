

#include <quan/uav/osd/api.hpp>
#include <quan/stm32/millis.hpp>

namespace quan{ namespace uav { namespace osd{

   void on_draw(){

        pxp_type pos;
        draw_text("This is a line",{-150,50});
        draw_text("This is another longer line",{-150,20});

//        draw_horizontal_line({-150,-20},100,colour_type::white);
//         draw_horizontal_line({-155,-30},300,colour_type::black);
//         draw_horizontal_line({-150,-40},60,colour_type::grey);
         auto const scr_sz = get_display_size();
         draw_horizontal_line({-scr_sz.x/2 + 8,scr_sz.y/2 - 12},scr_sz.x - 16, colour_type::black);
         draw_horizontal_line({-scr_sz.x/2 + 8,scr_sz.y/2 - 13},scr_sz.x - 16, colour_type::black);
         draw_horizontal_line({-scr_sz.x/2 + 8,-scr_sz.y/2 + 13},scr_sz.x - 16, colour_type::white);
         draw_horizontal_line({-scr_sz.x/2 + 8,-scr_sz.y/2 + 12},scr_sz.x - 16, colour_type::white);


   void on_draw(){
        if (def_font == nullptr){
          def_font = quan::uav::osd::get_font(FontID::OSD_Charset);
        }
      #if defined QUAN_OSD_TELEM_RECEIVER
        pxp_type pos1{-150,65};
        draw_text("Receiver",pos1,def_font);

        pxp_type pos{-150,-50};
        const char* telemetry_text = mutex_acquire_telemetry_string();
        if ( telemetry_text != nullptr){
         strcpy(telem_buffer,telemetry_text);
         mutex_release_telemetry_string();
        }
 
        draw_text(telem_buffer,pos,def_font);
      #else
        #if defined QUAN_OSD_TELEM_TRANSMITTER
        pxp_type pos{-150,50};
        draw_text("Transmitter",pos,def_font);
        #else
        // #error not tx or rx
          draw_text("Delay Test",{-150,30});
        

         draw_box({-20,-20},{20,20},colour_type::black,true);

         draw_box({20,20},{40,40},colour_type::black,false);

         draw_line(
            {-scr_sz.x/2 + 8,scr_sz.y/2 - 15},
            {-scr_sz.x/2 + 8,-scr_sz.y/2 + 15},
            colour_type::white
         );

         draw_line(
            {scr_sz.x/2 - 8,scr_sz.y/2 - 15},
            {scr_sz.x/2 - 8,-scr_sz.y/2 + 15},
            colour_type::black
         );
        auto const start_wait = quan::stm32::millis();
        while ( (quan::stm32::millis() - start_wait)  < quan::time_<int64_t>::ms(2000))
         { asm volatile("nop":::);}
         #endif
      #endif

   }
}}}