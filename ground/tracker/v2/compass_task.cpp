#include "resources.hpp"
#include "compass.hpp"

/*
 todo add atod in this task
*/

namespace{

      void compass_task(void * param)
      {
         raw_compass::init();
         for(;;){
            raw_compass::update();
         }
      }

      char dummy_param  = 0;
      TaskHandle_t task_handle = NULL;
}

void create_compass_task()
{

   xTaskCreate(
      compass_task,"compass_task", 
      2000,
      &dummy_param,
      local_task_priority::compass,
      &task_handle
   );
}