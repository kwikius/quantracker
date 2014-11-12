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