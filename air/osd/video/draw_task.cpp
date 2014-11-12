#include "graphics_api.hpp"
//#include "video.hpp"
#include "video_buffer.hpp"
#include <quan/dynarray.hpp>
#include "../resources.hpp"

//void get_data_to_transmit(quan::dynarray<uint8_t> & ar);
//void set_text_data( const char* text);

void swap_osd_buffers();
void create_osd_swap_semaphores();
void draw_loop();
namespace {
#if 0
   void draw_task(void * params)
   {
      create_osd_swap_semaphores();
      TickType_t last_wakeup = xTaskGetTickCount();
       for (;;){
        // vTaskDelayUntil(&last_wakeup,200);
        // asm volatile("nop":::);
         on_draw();
         swap_osd_buffers();
      }
   }
#else
   void draw_task(void * params)
   {
    //  create_osd_swap_semaphores();
     // TickType_t last_wakeup = xTaskGetTickCount();
       for (;;){
         draw_loop();
         vTaskDelay(2);
       }
   }
#endif

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