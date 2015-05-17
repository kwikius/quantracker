
#include "resources.hpp"

namespace {

      typedef sliprings_tx_rx_task sp_task;

      void tracker_mode(void * param)
      {
         
         sp_task::enable();
         bool cmdline_mode = true;
         // look for 3 x ret
         for (int count = 0;count < 3; ++count){
           if ( sp_task::get() != '\r'){
               cmdline_mode = false;
               break;
           }
         }
         if ( cmdline_mode){
            sp_task::write("in cmdline mode\n");
         }else{
            sp_task::write ("not cmdline mode\n");
         }
         for (;;){
             quan::stm32::set<heartbeat_led_pin>();
            asm volatile ("nop":::);
         }
      }

      char dummy_param  =0;
      TaskHandle_t task_handle = NULL;
}

void create_tracker_mode_task()
{

   sp_task::setup<9600>(local_interrupt_priority::sliprings_serial_port);
   xTaskCreate(
      tracker_mode,"tracker_mode", 
      5000,
      &dummy_param,
      local_task_priority::tracker_mode,
      &task_handle
   );
}