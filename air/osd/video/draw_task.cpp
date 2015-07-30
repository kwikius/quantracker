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

#include <quan/time.hpp>
#include <quan/uav/osd/api.hpp>
#include "osd_state.hpp"
#include "../resources.hpp"


namespace detail{
   void swap_osd_buffers();
   bool swap_osd_buffers(quan::time::ms const & wait_time);
   void create_osd_swap_semaphores();

   
}
namespace {
   int count =0;
   void draw_task(void * params)
   {
       vTaskDelay(100); // want to know if have video
       // take the semaphore?
       for (;;){
        
         quan::uav::osd::on_draw();

         if ( osd_state::get() == osd_state::internal_video ){
           // vTaskDelay(20);
            if ( ++count >= 25){
               count = 0;
               quan::stm32::complement<heartbeat_led_pin>();
            } 
            if ( osd_state::have_external_video()){
               count = 0;
               osd_state::set(osd_state::external_video);
            }else{
               detail::swap_osd_buffers();
               continue;
            }
         }
//         if ( osd_state::get() == osd_state::suspended){
//         //    vTaskDelay(20);
//            if ( ++count >= 35){
//               count = 0;
//               quan::stm32::complement<heartbeat_led_pin>();
//            } 
//         }
         if ( osd_state::get() == osd_state::external_video ){  
            if ( ++count >= 50){
               count = 0;
               quan::stm32::complement<heartbeat_led_pin>();
            }
            constexpr quan::time::ms wait_time{1000};
            if (!detail::swap_osd_buffers(wait_time)){
               count = 0;
               osd_state::set(osd_state::internal_video);
            }
         }

         
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
}

void create_draw_task()
{
    detail::create_osd_swap_semaphores();

   xTaskCreate(
      draw_task,"draw_task", 
      5000, 
      &dummy_param,
      task_priority::draw,
      &task_handle
   );
   // wait for 
}
