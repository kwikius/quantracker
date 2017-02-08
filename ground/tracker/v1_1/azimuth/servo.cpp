

#include <quan/stm32/tim.hpp>
#include <quan/stm32/get_raw_timer_frequency.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/stm32/push_pop_fp.hpp>

#include <quan/min.hpp>
#include "system/resources.hpp"

#include "../azimuth/servo.hpp"
#include "../azimuth/motor.hpp"
#include "../azimuth/encoder.hpp"

namespace {
   QUAN_ANGLE_LITERAL(rad)
   QUAN_ANGLE_LITERAL(deg)
   QUAN_QUANTITY_LITERAL(time,ms)
}

constexpr quan::time::us  azimuth_servo::m_pwm_period;
// set the prescaler resolution
constexpr quan::time::ns  azimuth_servo::m_timer_resolution;
// the time required to do the pid calculation and set the next PWM value
//  before the start of the next PWM cycle
// need to test that
constexpr quan::time::us azimuth_servo::m_max_calculation_time;

quan::irq_atomic_buffer<azimuth_servo::target_t> 
azimuth_servo::m_target_buffer{
    {0_rad,azimuth_servo::rad_per_s{0_rad}},
    {0_rad,azimuth_servo::rad_per_s{0_rad}} 
};

azimuth_servo::last_t azimuth_servo::m_last {0_rad,azimuth_servo::rad_per_s{0_rad},0_ms};
bool azimuth_servo::m_enabled = false;
bool azimuth_servo::m_is_reversed = false;

float azimuth_servo::m_kP = 2.5;
float azimuth_servo::m_kD = 0.22; 
float azimuth_servo::m_kI = 0.3f;
float azimuth_servo::m_ki = 1.f;

azimuth_servo::mode_t azimuth_servo::m_mode = azimuth_servo::mode_t::pwm;

bool azimuth_servo::enable()
{
   if ( ((m_mode == mode_t::position) || ( m_mode == mode_t::position_and_velocity)) 
           && !azimuth_encoder::is_indexed() ){
      gcs_serial::write("cannot enable servo until encoder indexed\n");
      return false;
   }else{
      azimuth_motor::enable();
      return true;
   }
}

void azimuth_servo::disable()
{
   azimuth_motor::disable();
}

quan::angle::rad 
azimuth_servo::get_current_bearing()
{
//   if ( ! azimuth_encoder::is_indexed()){
//      gcs_serial::write("WARNING: encoder index has not been set\n");
//   }
   return azimuth_encoder::encoder_to_bearing(azimuth_encoder::get_index());
}

namespace {
   // get angle difference between old and new allowing for overflow etc
   // a positive difference means new angle is greater than old angle
   quan::angle::rad get_angle_diff( quan::angle::rad const & old_bearing, quan::angle::rad const & new_bearing)
   {
      quan::angle::rad const diff = unsigned_modulo(new_bearing) - unsigned_modulo(old_bearing);
      if ( diff < -quan::angle::pi){
         return diff + 2 * quan::angle::pi;
      }else{
         if (diff > quan::angle::pi){
            return diff - 2 * quan::angle::pi;
         }else {
            return diff;
         }
      }
   }
}

/*
 angular_velocity clockwise is positive e.g a north to north east move is a positive move
*/
azimuth_servo::rad_per_s 
azimuth_servo::get_current_angular_velocity()
{
   auto const dt = quan::stm32::millis() - m_last.at_time;
   if ( dt < 1_ms){
      return m_last.angular_velocity;
   }else{
      return  get_angle_diff(m_last.bearing,get_current_bearing()) / dt;
   }
}

// in irq
azimuth_servo::rad_per_s 
azimuth_servo::get_update_angular_velocity_from_irq()
{
   auto const now = quan::stm32::millis_from_isr();
   auto const dt = now - m_last.at_time;
   if ( dt < 1_ms){
      return m_last.angular_velocity;
   }else{
      auto bearing = get_current_bearing();
      azimuth_servo::rad_per_s const result = get_angle_diff(m_last.bearing,get_current_bearing()) / dt;
      m_last.bearing = bearing;
      m_last.at_time = now;
     m_last.angular_velocity = result;
     return result;
   }
}

/*
   In proportional mode, set pwm directly
   A positive pwm should move the azimuth clockwise e.g a north to north east move is a positive move
*/
#if defined abs 
#error is a macro
#endif
namespace {
   float constexpr min_pwm = 0.1f;
};
bool azimuth_servo::set_pwm(float value_in)
{
   float const abs_value = std::abs(value_in);
   if (abs_value > min_pwm){
      bool const sign = value_in >= 0.f;
      float const value = quan::min(abs_value,0.99f);
      uint32_t const pwm_value = static_cast<uint32_t>(value * get_calc_compare_irq_value());
      azimuth_motor::set_pwm(pwm_value,sign);
   }else{
      azimuth_motor::set_pwm(0.f,true);
   }
   return true;
}

namespace{
   float kI = 0.f;

}

void azimuth_servo::position_irq()
{
    auto const position_error = signed_modulo(get_target_bearing() - get_current_bearing());
    auto const angular_velocity = get_update_angular_velocity_from_irq();

    auto sign = [](float x) { return x >= 0.f ? 1: -1;};
    
    if ( abs(position_error) < 5_deg) {
      kI += get_kI() * position_error.numeric_value() / 
         (std::abs(quan::pow<2>(angular_velocity.numeric_value().numeric_value()) ) + get_ki()) * sign(angular_velocity.numeric_value().numeric_value());
    }else{
      kI = 0.f;
    }

    set_pwm( get_kP() * position_error.numeric_value() - get_kD() * angular_velocity.numeric_value() + kI );
    azimuth_motor::set_pwm_irq();
}

void azimuth_servo::update_irq()
{
   quan::stm32::push_FPregs();

   switch( get_mode()){
      case mode_t::pwm:
         azimuth_motor::set_pwm_irq();
         break;
      case mode_t::position:
         position_irq();
         break;
      case mode_t::velocity:
      case mode_t::position_and_velocity:
      default:
         break;
   }
   quan::stm32::pop_FPregs();
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