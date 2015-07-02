
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

#include <FreeRTOS.h>
#include <task.h>

extern "C" void setup();

void create_mavlink_task();
void create_heartbeat_task();
void create_frsky_task();
void create_fsk_task();
void create_draw_task();

void mode_check();

int main()
{

  mode_check();
 
  setup();

  create_mavlink_task();
  create_frsky_task();
  create_heartbeat_task();
  create_fsk_task();
  create_draw_task();

  vTaskStartScheduler();

  while (1) {;}
}
