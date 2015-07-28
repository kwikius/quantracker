

#include <quan/uav/osd/api.hpp>

namespace quan{ namespace uav { namespace osd{

   void on_draw(){

        pxp_type pos;
        draw_text("This is a line",{-150,50});
        draw_text("This is another longer line",{-150,20});

        draw_line({-150,-20},{120,-30},colour_type::white);

   }
}}}