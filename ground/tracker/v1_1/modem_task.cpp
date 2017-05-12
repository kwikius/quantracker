

#include "FreeRTOS.h"
#include <task.h>
#include <quan/stm32/millis.hpp>
#include "system/resources.hpp"
#include <quan/uav/osd/api.hpp>
#include <quan/tracker/zapp4/position.hpp>
#include <quan/uav/cobs/packet_parser.hpp>

namespace {

  QUAN_QUANTITY_LITERAL(time,ms)

  quan::uav::cobs::packet_parser* packet_parser = nullptr;
  QueueHandle_t modem_telem_queue_handle = nullptr;

  void parse_data(char ch);

}

QueueHandle_t get_modem_telem_queue_handle() { return modem_telem_queue_handle;}


namespace {

   void setup_modem_parser()
   {
      packet_parser = new quan::uav::cobs::packet_parser{19}; // set to the size of the largest encoded packet
      modem_telem_queue_handle = xQueueCreate(1,sizeof(quan::uav::osd::norm_position_type));
   }

   void modem_task(void * params)
   {

      // allow the modem time to startup
      auto now = quan::stm32::millis();
      while ( (quan::stm32::millis() - now ) < 200_ms){asm volatile ("nop":::);}

      // init the serial port
      modem_serial::setup<115200>(local_interrupt_priority::modem_serial_port);
      modem_serial::enable();
      setup_modem_parser();

      ///----------------------

      for(;;){ 
         // get the next char ( blocking)
         char const ch = modem_serial::get();
         parse_data(ch);
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
                  if (result && (modem_telem_queue_handle != nullptr)){
                     xQueueOverwrite(modem_telem_queue_handle,&pos);
                  }
               }
            }
               break;
            default:{
               break;
            }
         }
      }
   }
}
