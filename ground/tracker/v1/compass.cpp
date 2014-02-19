

#include <stm32f4xx.h>
#include <quan/stm32/rcc.hpp>
#include <quan/stm32/f4/exti/set_exti.hpp>
#include <quan/stm32/f4/syscfg/module_enable_disable.hpp>

#include "resources.hpp"
#include "events.hpp"
#include <quan/three_d/vect.hpp>

namespace {

   uint32_t strap_value;
   void null_mag_fun(quan::three_d::vect<int16_t> const & result_in){};
   void (*pfn_mag)(quan::three_d::vect<int16_t> const &)=null_mag_fun;
}
void set_mag_strap(int32_t val){ strap_value = val;}

void set_mag_fun( void (*pfn)(quan::three_d::vect<int16_t> const &))
{
   pfn_mag = pfn;
}

int32_t get_mag_strap()
{
   return strap_value;
}

void on_new_mag( quan::three_d::vect<int16_t> const & result_in)
{
   pfn_mag(result_in);
}

namespace {

   void setup_mag_ready_irq()
   {
      quan::stm32::module_enable<quan::stm32::syscfg>(); 
      quan::stm32::set_exti_syscfg<mag_rdy_exti_pin>();
      quan::stm32::set_exti_falling_edge<mag_rdy_exti_pin>();
      NVIC_SetPriority(I2C1_EV_IRQn,interrupt_priority::exti_mag_rdy);
      quan::stm32::nvic_enable_exti_irq<mag_rdy_exti_pin>();
      quan::stm32::module_enable<mag_rdy_exti_pin::port_type>();
      quan::stm32::apply<
         mag_rdy_exti_pin
         , quan::stm32::gpio::mode::input
         , quan::stm32::gpio::pupd::none
      >();
      
      quan::stm32::enable_exti_interrupt<mag_rdy_exti_pin>();
   }
}

void setup_compass()
{
   setup_mag_ready_irq();
   NVIC_SetPriority(I2C1_EV_IRQn,interrupt_priority::i2c_mag_evt);
   i2c_mag_port::init(true,false);
   
}

extern "C" void I2C1_EV_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void I2C1_EV_IRQHandler()
{     
   static_assert(std::is_same<i2c_mag_port::i2c_type, quan::stm32::i2c1>::value,"incorrect port irq");
   i2c_mag_port::handle_irq();
}

extern "C" void I2C1_ER_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void I2C1_ER_IRQHandler()
{
   static_assert(std::is_same<i2c_mag_port::i2c_type, quan::stm32::i2c1>::value,"incorrect port irq");
   uint32_t const sr1 = i2c_mag_port::i2c_type::get()->sr1.get();
   i2c_mag_port::i2c_type::get()->sr1.set(sr1 & 0xFF); 
   i2c_mag_port::i2c_errno = i2c_mag_port::errno_t::i2c_err_handler;
}

extern "C" void EXTI1_IRQHandler() __attribute__ ((interrupt ("IRQ")));
extern "C" void EXTI1_IRQHandler()
{      
   if (quan::stm32::is_event_pending<mag_rdy_exti_pin>()){
      mag_rdy_event.set();
      quan::stm32::clear_event_pending<mag_rdy_exti_pin>();
   }else{
      i2c_mag_port::i2c_errno = i2c_mag_port::errno_t::unknown_exti_irq;
   }
}
