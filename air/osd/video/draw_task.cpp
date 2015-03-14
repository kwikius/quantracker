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

#include "../resources.hpp"
#include <quan/uav/osd/api.hpp>

void swap_osd_buffers();
void create_osd_swap_semaphores();

namespace {

   void draw_task(void * params)
   {
       create_osd_swap_semaphores();
       for (;;){
         quan::uav::osd::on_draw();
         swap_osd_buffers();
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
}

void create_draw_task()
{

#if (SWDIO_DEBUG == SWDIO_DEBUG_DRAW_TASK)
	  quan::stm32::module_enable<swdio::port_type>();

	   quan::stm32::apply<
	      swdio
	      , quan::stm32::gpio::mode::output
	      , quan::stm32::gpio::otype::push_pull
	      , quan::stm32::gpio::pupd::none
	      , quan::stm32::gpio::ospeed::slow
	      , quan::stm32::gpio::ostate::low
	   >();
#endif


   xTaskCreate(
      draw_task,"draw_task", 
      2500,
      &dummy_param,
      task_priority::draw,
      &task_handle
   );
}
