

#include <quan/stm32/tim.hpp>
#include <quan/stm32/get_raw_timer_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/stm32/push_pop_fp.hpp>

#include <quan/min.hpp>
#include "system/resources.hpp"

#include "../azimuth/servo.hpp"
#include "../azimuth/motor.hpp"
#include "../azimuth/encoder.hpp"

constexpr quan::time::us  azimuth_servo::m_pwm_period;
// set the prescaler resolution
constexpr quan::time::ns  azimuth_servo::m_timer_resolution;
// the time required to do the pid calculation and set the next PWM value
//  before the start of the next PWM cycle
// need to test that
constexpr quan::time::us azimuth_servo::m_max_calculation_time;

// to make update atomic
azimuth_servo::targets_t azimuth_servo::target_arr[2] = 
{
  {quan::angle::rad{0},azimuth_servo::rad_per_s{quan::angle::rad{0}}}
  ,{quan::angle::rad{0},azimuth_servo::rad_per_s{quan::angle::rad{0}}}
};

uint8_t azimuth_servo::m_current_target_idx = 0U;

bool azimuth_servo::m_enabled = false;

bool azimuth_servo::enable()
{
   if (azimuth_encoder::is_indexed()){
      azimuth_motor::enable();
      return true;
   }else{
      gcs_serial::write("cannot enable servo until encoder indexed\n");
      return false;
   }
}

void azimuth_servo::disable()
{
   azimuth_motor::disable();
}

void azimuth_servo::set_pwm(float value_in)
{
   
   bool const sign = value_in >= 0.f;
   float const value = quan::min(std::abs(value_in),0.99f);
   uint32_t const pwm_value = static_cast<uint32_t>(value * get_calc_compare_irq_value());
  /* debug gcs_serial::print<120>("in value = %f, value = %f,pwm = %lu, sign = %lu\n",
      static_cast<double>(value_in),
      static_cast<double>(value),
      pwm_value, 
      static_cast<uint32_t>(sign)
   );
 */
   azimuth_motor::set_pwm(pwm_value,sign);
}

void azimuth_servo::ll_update_irq()
{
   quan::stm32::push_FPregs();

   azimuth_motor::set_pwm_irq();

   quan::stm32::pop_FPregs();
}

void azimuth_servo::update_irq()
{
/*
   if ( ! azimuth_encoder::is_indexed()){
      azimuth_motor::disable();
      return ;
   }
   if ( ! is_enabled() ){
       azimuth_motor::disable();
       return;
   }
*/
   ll_update_irq();

}

void azimuth_servo::setup()
{
   setup_timer();
   setup_update_interrupt();
   azimuth_encoder::setup();
   azimuth_motor::setup();

   timer::get()->cr1.setbit<0>(); //(CEN)
}

void azimuth_servo::setup_timer()
{
   quan::stm32::module_enable<timer>();
   quan::stm32::module_reset<timer>();

   constexpr quan::frequency::Hz timer_raw_freq{quan::stm32::get_raw_timer_frequency<timer>()};
   // presc is -1 to the value but subtract 0.9 so get integer round off
   auto constexpr psc = static_cast<uint32_t>( timer_raw_freq  *  m_timer_resolution - 0.9f);
   static_assert( psc == 167U,"odd value");
   // arr is -1 to the value but subtract 0.9 so get integer round off
   auto constexpr arr = static_cast<uint32_t>(( m_pwm_period / m_timer_resolution) - 0.9f);
   static_assert(arr == 19999,"odd value");

   timer::get()->psc = psc;
   timer::get()->rcr = 0;
   timer::get()->arr = arr;
   timer::get()->cnt = 0x0;

   {
      quan::stm32::tim::cr1_t cr1 = 0U;
      cr1.urs = true; // only update generates update interrupt
      cr1.arpe = true;
      timer::get()->cr1.set(cr1.value);
      // n.b timer disabled
   }

   timer::get()->cr2.set(0);
   timer::get()->smcr.set(0);
   timer::get()->egr.set(0);

}

void azimuth_servo::setup_update_interrupt()
{
   {
      // enable compare on channel 1
      quan::stm32::tim::ccmr1_t ccmr1 = timer::get()->ccmr1.get();
      // compare1 is output but not mapped toa pin
      // only want an interrupt
      ccmr1.cc1s = 0b00; // ch1 is output
      ccmr1.oc1m = 0b000;  // frozen output so timebase only
      timer::get()->ccmr1.set(ccmr1.value);
   }

   // set compare to happen calctime before end of pwm period
  // constexpr quan::time::us calc_interrupt_time = m_pwm_period - m_max_calculation_time;
  // constexpr uint32_t calc_compare_irq_value = static_cast<uint32_t>( (calc_interrupt_time / m_timer_resolution) - 0.9f );

   timer::get()->ccr1 = get_calc_compare_irq_value();
   
   // enable channel 1 compare
   timer::get()->ccer.setbit<0>(); // (CC1E)

   timer::get()->dier.setbit<1>(); // (CC1IE)

   timer::get()->sr = 0;
   NVIC_SetPriority(TIM1_CC_IRQn, local_interrupt_priority::pan_motor);

   NVIC_EnableIRQ(TIM1_CC_IRQn);
   
  //N.B timer needs to be enabled (CEN) to run
}

extern "C"  void TIM1_CC_IRQHandler() __attribute__ ((interrupt ("IRQ")));

  
extern "C"  void TIM1_CC_IRQHandler()
{
   auto const sr = azimuth_servo::timer::get()->sr.get();
   if ( sr & (1<<1) ){
      azimuth_servo::timer::get()->sr.clearbit<1>(); // (CC1IF)
      azimuth_servo::update_irq();
      return;
   }
   // panic
    // Problem!
}