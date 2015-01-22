/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korbonen(Openpilot.org)
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

#include <cmath>
#include <cstring>
#include <cstdio>
 
#include "video_cfg.hpp"
#include "graphics_api.hpp"
#include "aircraft.hpp"
 
#if 1
namespace {
 
   typedef quan::two_d::vect<float> fvect;
   typedef quan::two_d::vect<uint32_t> uvect;
    
   fvect cursor = fvect {50.f,120.f};
   char text_buffer[200] = {0};
   void draw_heading()
   {
        sprintf(text_buffer,"heading = %ld deg",static_cast<int32_t>(the_aircraft.heading.numeric_value()));
        draw_text(cursor,text_buffer);
   }

   void draw_yaw_pitch_roll()
   {
        sprintf(text_buffer,"pitch, roll, yaw (deg) = %ld,%ld,%ld"
                ,static_cast<int32_t>(the_aircraft.attitude.pitch.numeric_value())
                ,static_cast<int32_t>(the_aircraft.attitude.roll.numeric_value())
                ,static_cast<int32_t>(the_aircraft.attitude.yaw.numeric_value())
               );
        draw_text(cursor,text_buffer);
   }

   void draw_num_sats()
   {
        const char* got_home = (the_aircraft.gps.has_home == true)?"true":"false";
        sprintf(text_buffer,"got home = %s,numsats = %ld",
                got_home,static_cast<int32_t>(the_aircraft.gps.num_sats)
               );
        draw_text(cursor,text_buffer);
   }
}
 
void on_draw()
{
     // uvect display_size = video_cfg::get_display_size_px();
     cursor = fvect {20.f,20.f};
     draw_heading();
     cursor += fvect {0.f,21.f};
     draw_yaw_pitch_roll();
     cursor += fvect {0.f,21.f};
     draw_num_sats();
}
#else

void on_draw()
{
   typedef quan::two_d::vect<float> vect;
   for ( uint32_t i = 0; i < 10;++i){

    draw_line_bres(vect{50*i +20,0},vect{50*i+20,400},color::black);
    draw_line_bres(vect{50*i +21,0},vect{50*i+21,400},color::black);

    draw_line_bres(vect{50*i +30,0},vect{50*i+30,400},color::black);
    draw_line_bres(vect{50*i +31,0},vect{50*i+31,400},color::black);
    }
}

#endif
 
 
 