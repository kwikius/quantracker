

#include "FreeRTOS.h"
#include "task.h"

#include <quan/time.hpp>
#include <quan/stm32/gpio.hpp>
#include "resources.hpp"

namespace {
   typedef quan::time_<int32_t>::ms ms;
}
struct abc_led_task_info {
     abc_led_task_info( bool init_state,ms high_time, ms low_time)
          : m_state {init_state},m_high_time {high_time}, m_low_time {low_time} {}

     ms get_low_time()const 
    {
          return m_low_time;
     }
     ms get_high_time()const 
    {
          return m_high_time;
     };
     bool is_low() const 
    {
          return m_state == false;
     }
     void set() 
     {
          ll_set();
          m_state = true;
     }
     void clear() 
     {
          ll_clear();
          m_state = false;
     }
protected:
     virtual void ll_set() = 0;
     virtual void ll_clear() = 0;
     virtual ~abc_led_task_info() {}
private:
     bool m_state;
     ms m_high_time;
     ms m_low_time;
} ;

template <typename Pin>
struct led_task_info : abc_led_task_info {
     led_task_info( bool init_state,ms high_time, ms low_time)
          : abc_led_task_info {init_state,high_time,low_time} {}
private:
     void ll_set() 
     {
          quan::stm32::set<Pin>();
     }
     void ll_clear() 
     {
          quan::stm32::clear<Pin>();
     }
};

void led_task( void *param)
{
     abc_led_task_info * led = static_cast<abc_led_task_info*>(param);
     for(;;) {
          if ( led->is_low()) {
               led->set();
               vTaskDelay( led->get_high_time().numeric_value());
          } else {
               led->clear();
               vTaskDelay( led->get_low_time().numeric_value());
          }
     }
}

template <typename Pin>
void init_led_task( unsigned portBASE_TYPE uxPriority, bool init_state,ms low, ms high)
{
     auto*  p = new led_task_info<Pin> {
          init_state,low,high
     };
     xTaskCreate( led_task, "LEDx",configMINIMAL_STACK_SIZE,
                  static_cast<abc_led_task_info*>(p),
                  uxPriority, ( TaskHandle_t * ) NULL );
}

void create_led_tasks( )
{
   unsigned portBASE_TYPE priority = task_priority::heartbeat;
   init_led_task<green_led_pin> (priority,false,ms{1000},ms{2000} );
  // init_led_task<red_led_pin> (priority,false,ms{2500},ms{250} );
  // init_led_task<orange_led_pin> (priority, false,ms{250},ms{250} ); 
  // init_led_task<blue_led_pin> ( priority, false,ms{1000},ms{1000} );
}
