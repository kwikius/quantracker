 /*
 Copyright (c) 2013-2015 Andy Little 

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

#include "graphics_api.hpp"
#include "video_buffer.hpp"
#include <quan/dynarray.hpp>
#include "../resources.hpp"

void swap_osd_buffers();
void create_osd_swap_semaphores();

namespace {

   void draw_task(void * params)
   {
       create_osd_swap_semaphores();
       for (;;){
         on_draw();
         swap_osd_buffers();
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
}

void create_draw_task()
{
   xTaskCreate(
      draw_task,"draw_task", 
      3000,
      &dummy_param,
      task_priority::draw,
      &task_handle
   );
}