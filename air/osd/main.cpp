
/*
 Copyright (c) 2013 Andy Little 

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

#ifndef QUAN_OSD_BOARD_TYPE
   #error which board?
#else
   #if QUAN_OSD_BOARD_TYPE == 1
      #pragma message "for board 1"
   #else 
      #if QUAN_OSD_BOARD_TYPE == 2
         #pragma message "for board 2"
      #else
         #if QUAN_OSD_BOARD_TYPE == 3
            #pragma message "for board 3"
         #else
            #error unknown board
         #endif
      #endif
   #endif
#endif

#include <cstdint>
#include "mavlink.hpp"
#include "gps.hpp"
#include "settings.hpp"
#include "events.hpp"
#include "resources.hpp"

extern "C" void setup();

namespace {

   void read_data()
   {
      read_mavlink();
   }
}

void do_tasks()
{
   read_data();
   service_events();
}

int main()
{
   setup();
   for(;;){
      draw_loop();
      do_tasks();
   }
}
