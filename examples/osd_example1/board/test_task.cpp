
/*
just a task to test the board is working. Prob should do a proper test suite to verify the board
*/

#include <cstdint>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "mavlink.hpp"

#include "aircraft.hpp"
#include "resources.hpp"


namespace {

 void comm_send_ch(uint8_t ch)
{
 #if  (QUAN_OSD_BOARD_TYPE == 4)
   mavlink_tx_rx_task::put(ch);
#else
  posdata_tx_rx_task::put(ch);
#endif
}
   void test_sp_output(void * )
   {
        mavlink_tx_rx_task::enable();
       for (;;){
         mavlink_tx_rx_task::put('B');
         quan::stm32::set<heartbeat_led_pin>(); 
         vTaskDelay(333);
         quan::stm32::clear<heartbeat_led_pin>(); 
         vTaskDelay(667);
       }
   }

   char dummy_param  =0;
   TaskHandle_t task_handle = NULL;
}//namespace

void create_test_task()
{
   xTaskCreate(
         test_sp_output,"test_output", 
         512,
         &dummy_param,
         task_priority::mavlink,
         &task_handle);
}