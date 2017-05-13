

#include "FreeRTOS.h"
#include <task.h>
#include <quan/stm32/millis.hpp>
#include "system/resources.hpp"
#include <quan/uav/osd/api.hpp>
#include <quan/tracker/zapp4/position.hpp>
#include <quan/uav/cobs/packet_parser.hpp>
#include "osd/on_telemetry_received.hpp"

namespace {

  QUAN_QUANTITY_LITERAL(time,ms)

  quan::uav::cobs::packet_parser* packet_parser = nullptr;

  void parse_data(char ch);

}

namespace {

   void setup_modem_parser()
   {
      packet_parser = new quan::uav::cobs::packet_parser{19}; // set to the size of the largest encoded packet
   }

   void modem_task(void * params)
   {
      // init the serial port
      setup_modem_parser();
      modem_serial::setup<115200>(local_interrupt_priority::modem_serial_port);
      modem_serial::enable();

      TickType_t xLastWakeTime = xTaskGetTickCount();

      // buffer data for 1/100 th sec or so
      TickType_t constexpr period = 11;

      for(;;){ 
         vTaskDelayUntil(&xLastWakeTime, period);
         size_t const n = modem_serial::in_avail();
         for(size_t i = 0; i < n; ++i){
            char const ch = modem_serial::get();
            parse_data(ch);
         }
      }
   }

   char dummy_param = 0;
   TaskHandle_t task_handle = NULL;
   constexpr uint32_t task_stack_size = 2000U;
   StackType_t __attribute__ ((used,section (".task_stacks"))) task_stack[task_stack_size];
   StaticTask_t __attribute__ ((used,section (".task_stacks"))) task_buffer;
}

void create_modem_task()
{
   task_handle = ::xTaskCreateStatic(
      modem_task
      ,"modem_task"
      ,task_stack_size
      ,&dummy_param
      ,local_task_priority::modem_task
      ,task_stack
      ,&task_buffer
   );
}

namespace {

   void parse_data(char ch)
   {
      uint16_t const packet_length = packet_parser->parse(ch);
      if ( packet_length > 0 ){
         uint8_t const * decoded_packet_buffer = packet_parser->get_decoded_packet();
         // check its length matches the command
         uint16_t command_id = decoded_packet_buffer[0];
         switch(command_id){
            case quan::tracker::zapp4::command_id::position:{
               if (packet_length == quan::tracker::zapp4::get_decoded_packet_size(command_id)){
                  quan::uav::osd::norm_position_type pos;
                  bool const result = quan::tracker::zapp4::get_position(packet_parser->get_decoded_packet(),pos);
                  if (result) {
                     auto q_handle = get_telem_queue_handle();
                     if ( q_handle != nullptr){
                        xQueueSendToBack(q_handle,&pos,0);
                     }
                  }
               }
               break;
            } 
            default:{
               gcs_serial::write("unknown rf_modem data \n");
               break;
            }
         }
      }
   }
}
