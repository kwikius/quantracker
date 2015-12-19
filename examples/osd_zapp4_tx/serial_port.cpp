
#include "resources.hpp"
#include <quan/uav/position.hpp>
#include <quan/tracker/zapp3_decode.hpp>
#include "tx_telemetry.hpp"
#include <quan/stm32/gpio.hpp>

extern "C" void USART1_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void USART1_IRQHandler()
{
   static_assert(
   std::is_same<
     mavlink_usart,quan::stm32::usart1
   >::value ,"invalid usart for serial_port irq");

   mavlink_tx_rx_task::irq_handler();
}

namespace {

   typedef quan::uav::position<
      quan::angle_<int32_t>::deg10e7,
      quan::length_<int32_t>::mm
   > pos_type;

   bool do_frame( pos_type & new_pos)
   {

      static uint8_t ar[16];
      static uint8_t idx = 0;
      
      uint8_t ch = mavlink_tx_rx_task::get();
      if (idx != 0) { // in frame
         if (ch != 0U) {
            ar[idx] = ch;
            if (++idx == 16U) {
               idx = 0U; // reset
               // if conv failed do error
               bool no_error = quan::tracker::zapp3_decode (ar,new_pos);
               if ( no_error){
                //  error_led.switch_off();
               }else{
                 // error_led.switch_on();
               }
               return no_error;
            } else {
               return false;
            }
         } else { // A zero in the encoded data means corruption
            idx = 0U;
           // error_led.switch_on();
            return false;
         }
      } else { // looking for start of frame
         if (ch == 0) { // start of frame
            ar[0] = 0U;
            idx = 1U;
         }
         return false;
      }
   }
  
   uint32_t frame_count = 0;
   void input_task(void* params)
   {
      mavlink_tx_rx_task::setup<9600>(interrupt_priority::telemetry_input_port);
      mavlink_tx_rx_task::enable();
      quan::stm32::set<heartbeat_led_pin>();

      for(;;){
         pos_type pos;
         if (do_frame(pos)){
            quan::uav::osd::norm_position_type * pos_ptr = mutex_acquire_position(5);
            if(pos_ptr){
              *pos_ptr = pos;
              mutex_release_position();
               ++frame_count;
              if( frame_count == 5){
                  frame_count = 0;
                  quan::stm32::complement<heartbeat_led_pin>();
              }
            }
         }
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;

}  // namespace

void create_input_task()
{

    xTaskCreate(
      input_task,"input_task", 
      512,
      &dummy_param,
      task_priority::mavlink,
      &task_handle
   );
  
}









