#include "resources.hpp"
#include "compass.hpp"

void create_mag_ready_semaphores();

namespace{

   

      void init_compass()
      {
         create_mag_ready_semaphores();
         raw_compass::init();
      }

      void compass_task(void * param)
      {
      
         init_compass();

         typedef sliprings_tx_rx_task sp;
         sp::enable();
         sp::write("I2C Test");
      //   quan::user_message("Testing I2C\n");
         for(;;){
         //   vTaskDelay(10);
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