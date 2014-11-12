#include <stm32f4xx.h>
#include <misc.h>
#include <quan/time.hpp>
#include <quan/frequency.hpp>
#include <quan/stm32/get_module_bus_frequency.hpp>
#include <quan/stm32/usart/irq_handler.hpp>
#include "video/video_cfg.hpp"
//#include "video/video.hpp"
#include "resources.hpp"
#include "fsk.hpp"
#include "frsky.hpp"

extern "C" void __cxa_pure_virtual()
{
     while (1);
}
void *__dso_handle;

extern "C" void vPortFree( void *pv );
extern "C"  void * pvPortMalloc(size_t n);

void operator delete (void* pv){ vPortFree(pv);}
void* operator new (unsigned int n){ return pvPortMalloc(n);}

void video_setup();
void setup_leds();

namespace {

   void setup_test_pin()
   {
      quan::stm32::module_enable< test_output_pin::port_type>();
         quan::stm32::apply<
            test_output_pin
            , quan::stm32::gpio::mode::output
            , quan::stm32::gpio::otype::push_pull
            , quan::stm32::gpio::pupd::none
            , quan::stm32::gpio::ospeed::slow
            , quan::stm32::gpio::ostate::low
         >();
   }
}
 
extern "C" void setup()
{
  
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  setup_test_pin();
  setup_leds();
  video_setup();
#if (QUAN_OSD_BOARD_TYPE == 2 )  || (QUAN_OSD_BOARD_TYPE == 3 )
  Dac_setup();
#endif
 // fsk::setup();
 // posdata_tx_rx_task::setup<57600>(interrupt_priority::telemetry_input_port);
 // frsky_tx_rx_task::setup<9600>(interrupt_priority::frsky_serial_port);
}
