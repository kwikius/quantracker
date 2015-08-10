

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
         
   }
}}}