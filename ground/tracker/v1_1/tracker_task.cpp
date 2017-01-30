
#include "system/resources.hpp"
#include "azimuth.hpp"
#include "FreeRTOS.h"
#include <task.h>
#include <cstdio>

namespace {

      void tracker_task(void * params)
      {
         azimuth::encoder::setup();

         for (;;){
            vTaskDelay(300);
            quan::stm32::complement<heartbeat_led_pin>();
            gcs_serial::print<100>("encoder count = %lu\n",azimuth::encoder::get_count());
         }
      }

      char dummy_param  =0;
      TaskHandle_t task_handle = NULL;

     constexpr uint32_t task_stack_size = 3000U;

     StackType_t __attribute__ ((used,section (".task_stacks"))) task_stack[task_stack_size];
     StaticTask_t __attribute__ ((used,section (".task_stacks"))) task_buffer;
}

void create_tracker_task()
{
   task_handle = ::xTaskCreateStatic(
     tracker_task,
     "tracker_task",
     task_stack_size,
     &dummy_param,
     local_task_priority::tracker_task,
     task_stack,
     &task_buffer);

}

