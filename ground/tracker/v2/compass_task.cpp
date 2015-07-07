#include "resources.hpp"
#include "compass.hpp"

namespace {

      SemaphoreHandle_t mag_ready_semaphore = NULL;
}
  
SemaphoreHandle_t get_mag_ready_semaphore()
{
    return mag_ready_semaphore;
}

namespace{

      void init_compass()
      {
         raw_compass::init();
         mag_ready_semaphore = xSemaphoreCreateBinary(); 
      }

      void compass_task(void * param)
      {
      
         init_compass();

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