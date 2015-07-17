
#include "resources/osd_resources.hpp"
#include <quan/stm32/flash.hpp>

bool initialise_flash()
{
  if (! quan::stm32::flash::get_app_symbol_table().init()){
      return false;
  }
  return true;
  // add bool init flash values
}

void signal_exit_failure()
{
   quan::stm32::module_enable< heartbeat_led_pin::port_type>();
      quan::stm32::apply<
         heartbeat_led_pin
         , quan::stm32::gpio::mode::output
         , quan::stm32::gpio::otype::push_pull
         , quan::stm32::gpio::pupd::none
         , quan::stm32::gpio::ospeed::slow
         , quan::stm32::gpio::ostate::high
      >();
   for(;;){asm volatile ("nop":::);}
}

