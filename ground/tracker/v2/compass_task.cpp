#include "resources.hpp"
#include "compass.hpp"



namespace {

     // SemaphoreHandle_t compass_semaphore = NULL;

      SemaphoreHandle_t mag_ready_semaphore = NULL;
}
  
SemaphoreHandle_t get_mag_ready_semaphore()
{
    return mag_ready_semaphore;
}
//SemaphoreHandle_t get_compass_semaphore()
//{
//   return compass_semaphore;
//}

namespace{

      void init_compass()
      {
           // send init values
         raw_compass::init();
        // compass_semaphore = xSemaphoreCreateBinary();
         mag_ready_semaphore = xSemaphoreCreateBinary();
         
      }

      void read_compass()
      {
         raw_compass::update();
      }

      void compass_task(void * param)
      {
      
         init_compass();

         for(;;){
            read_compass();
         }
         
      }

      char dummy_param  = 0;
      TaskHandle_t task_handle = NULL;
}

void create_compass_task()
{

   xTaskCreate(
      compass_task,"compass_task", 
      5000,
      &dummy_param,
      local_task_priority::compass,
      &task_handle
   );
}