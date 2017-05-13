
/*
 Copyright (c) 2017 Andy Little 

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

#include "FreeRTOS.h"
#include <task.h>

#include "system/common_resources.hpp"
#include "system/tracker_resources.hpp"

namespace{

   uint32_t m_button_count = 0;
   constexpr uint32_t m_max_button_count = 5;

   enum class m_button_state : bool {up, down};
     
   m_button_state m_button = m_button_state::up;

   volatile bool m_button_pressed = false;

   void m_setup_button()
   {
      quan::stm32::module_enable<user_button_pin::port_type>();
      quan::stm32::apply<
         user_button_pin
         , quan::stm32::gpio::mode::input
         , quan::stm32::gpio::pupd::pull_up
      >();

      m_button = m_button_state::up;
      m_button_count = 0;
   }

   bool m_button_is_down() {return quan::stm32::get<user_button_pin>() == false;}
   bool m_button_is_up() {return quan::stm32::get<user_button_pin>() == true;}

   void m_button_press_task( void * params)
   {
      m_setup_button();
 
      TickType_t lastWakeTime = xTaskGetTickCount();
      TickType_t constexpr not_pressed_delay = 10U;
      TickType_t constexpr pressed_delay = 100U;
      for (;;){
         if ( ! m_button_pressed){
            vTaskDelayUntil(&lastWakeTime,not_pressed_delay);
            switch( m_button ){
               case m_button_state::up:
                  if ( m_button_is_down() ){
                     if(++m_button_count == m_max_button_count){
                        m_button = m_button_state::down;
                        m_button_count = 0U;
                     }
                  }else{
                     m_button_count = 0U;
                  }
               break;
               case m_button_state::down:
                  if ( m_button_is_up()){
                     if (++ m_button_count == m_max_button_count){
                        m_button = m_button_state::up;
                        m_button_count = 0U;
                        m_button_pressed = true;
                        gcs_serial::write("button pressed\n");
                     }
                  }else{
                     m_button_count = 0;
                  }
               break;
            }
         }else{
            vTaskDelayUntil(&lastWakeTime,pressed_delay);
         }
      }
   }

   char m_dummy_param = 0;
   TaskHandle_t m_task_handle = NULL;
   constexpr uint32_t m_task_stack_size = configMINIMAL_STACK_SIZE;
   constexpr uint32_t m_task_priority = local_task_priority::button_press_task;
   StackType_t __attribute__ ((used,section (".task_stacks"))) m_task_stack[m_task_stack_size];
   StaticTask_t __attribute__ ((used,section (".task_stacks"))) m_task_buffer;
}

bool button_pressed() { return m_button_pressed;} // atomic
void clear_button_pressed() { m_button_pressed = false;} // atomic

void create_button_press_task()
{
   m_task_handle = ::xTaskCreateStatic(
      m_button_press_task
      ,"button_press"
      ,m_task_stack_size
      ,&m_dummy_param
      ,m_task_priority
      ,m_task_stack
      ,&m_task_buffer
   );
}


