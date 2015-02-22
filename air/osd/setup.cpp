
/*
 Copyright (c) 2013 -2015 Andy Little 

 With Grateful Acknowledgements to the prior work of:
   Sami Korhonen(Openpilot.org)
   taulabs ( taulabs.com) 
   brainFPV ( brainfpv.com)
   Thomas Oldbury (super-osd.com)

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
#if (QUAN_OSD_BOARD_TYPE != 1 ) 
void Dac_setup();
// for 8 bit only msbyte of val is used
// code is 00 write to specific reg but dont update
// 1 is write to specific reg and update outputs
// 2 is write tao all registers and update outputs
// 3 is power down outputs
// really only 0 1nd 1 are useful
void Dac_write(uint8_t ch, quan::voltage::V const & vout, uint8_t code);

#endif

namespace {

#if 0
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
#endif
}
 
extern "C" void setup()
{
  
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
 // setup_test_pin();
  setup_leds();
  video_setup();
#if (QUAN_OSD_BOARD_TYPE != 1 )
  Dac_setup();
#endif
  fsk::setup();
 #if QUAN_OSD_BOARD_TYPE == 4
  mavlink_tx_rx_task::setup<57600>(interrupt_priority::telemetry_input_port);
#else
  posdata_tx_rx_task::setup<57600>(interrupt_priority::telemetry_input_port);
#endif
  frsky_tx_rx_task::setup<9600>(interrupt_priority::frsky_serial_port);
}
