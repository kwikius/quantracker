
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


#include <stm32f4xx.h>

#include <FreeRTOS.h>
#include <task.h>

#include "resources.hpp"
#include "video/graphics_api.hpp"
#include "video/video_buffer.hpp"

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

extern "C" void setup();

void create_mavlink_task();
void create_heartbeat_task();
void create_frsky_task();
void create_fsk_task();
void create_draw_task();
void create_telem_task();
void create_led_tasks( );

void draw_loop();

int main()
{
  setup();

  //on_draw();
 // video_buffers::osd::manager.swap(); 

//  create_mavlink_task();
//  create_frsky_task();
//  create_heartbeat_task();
//  create_fsk_task();
//  create_draw_task();
// // create_telem_task();
//  create_led_tasks();
//
//  vTaskStartScheduler();

  while (1) {draw_loop();}
}
