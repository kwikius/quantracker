
/*
 Copyright (c) 2013 -2015 Andy Little 

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
//#include "video/graphics_api.hpp"
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
            #if QUAN_OSD_BOARD_TYPE == 4
               #pragma message "for board 4"
            #else
               #error unknown board
            #endif
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

#if defined QUAN_OSD_TELEM_TRANSMITTER
void create_vsync_telem_tx_task();
void create_osd_suspend_task();
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
void create_vsync_telem_rx_task();
#endif

#if ( QUAN_OSD_BOARD_TYPE !=4)
void create_leds_task();
#endif

int main()
{
  setup();
  create_draw_task();
#if defined QUAN_OSD_TELEM_TRANSMITTER
  create_vsync_telem_tx_task();
  create_osd_suspend_task();
#endif
#if defined QUAN_OSD_TELEM_RECEIVER
  create_vsync_telem_rx_task();
#endif

  vTaskStartScheduler();

  while (1) {;}
}
