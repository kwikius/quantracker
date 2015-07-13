
#include <cstdio>
#include "compass.hpp"
#include "resources.hpp"

namespace {
   typedef sliprings_tx_rx_task sp_task;
}

void compass_calibration()
{

   sp_task::write("\nrotate compass\n");

   quan::time_<int64_t>::ms const end_time 
      = quan::stm32::millis() + quan::time_<int64_t>::ms{15000};

  // bool start = true;
   quan::three_d::vect<float> min_values{0.f,0.f,0.f};
   quan::three_d::vect<float> max_values{0.f,0.f,0.f};
   while ( quan::stm32::millis() < end_time){
      taskENTER_CRITICAL();
      if ( raw_compass::acquire_mutex(1) == pdTRUE){
         quan::three_d::vect<float> compass_vect = raw_compass::get_raw();
         raw_compass::release_mutex();
         taskEXIT_CRITICAL();
         char buf [100];
         sprintf(buf,"[%.3f,%.3f,%.3f]\n",
            static_cast<double>(compass_vect.x),
               static_cast<double>(compass_vect.y),
               static_cast<double>(compass_vect.z));
         sp_task::write(buf);

      //vTaskSuspend(5);
     }else{
        taskEXIT_CRITICAL();
     }
   }

}