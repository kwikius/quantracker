
#include "FreeRTOS.h"
#include <task.h>

#include "system/resources.hpp"
#include "azimuth/encoder.hpp"
#include "azimuth/servo.hpp"

void parse_commandline();

namespace {

   void tracker_task(void * params)
   {
      gcs_serial::write("starting tracker task\n");

      for(;;){ 
         parse_commandline();
      }
   }

   char dummy_param = 0;
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
      &task_buffer
   );
}
