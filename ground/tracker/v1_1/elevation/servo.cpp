

#include <quan/max.hpp>
#include <quan/min.hpp>

#include <quan/stm32/tim.hpp>
#include <quan/stm32/tim/temp_reg.hpp>
#include <quan/stm32/gpio.hpp>

#include "../elevation/servo.hpp"
#include "../azimuth/servo.hpp"


namespace {
   QUAN_ANGLE_LITERAL(rad)
   QUAN_ANGLE_LITERAL(deg)
   QUAN_QUANTITY_LITERAL(time,us)
}

void elevation_servo::set_elevation(quan::angle::rad const & angle_in)
{
   auto const angle = quan::max(quan::min(angle_in, 90_deg),0_deg);
   auto const pulse_0  = 1000_us;
   auto const pulse_90 = 2000_us;
   auto const pulse_time = pulse_0 + (pulse_90 - pulse_0) * angle / 90_deg;
   
   typedef azimuth_servo::timer timer;

   timer::get()->ccr4 = static_cast<uint32_t>(pulse_time / azimuth_servo::m_timer_resolution) ;

}

// TIM1_CH4 PA11
/*
   assume basic timer is already set
*/
using namespace quan::stm32;

void elevation_servo::setup()
{
   module_enable<elevation_servo_pwm_out_pin::port_type>();

   apply<
      elevation_servo_pwm_out_pin
      ,gpio::mode::af1     
      ,gpio::pupd::none  
      ,gpio::ospeed::slow 
   >();
   
   typedef azimuth_servo::timer timer;

   // set CH4 to PWM mode
   // init value to 0
   timer::get()->ccr4 = 0;

   {
      tim::ccmr2_t ccmr2 = timer::get()->ccmr2.get();
         ccmr2.cc4s = 0b00; // channel 3 output
         ccmr2.oc4m = 0b110;  // PWM mode 1
         ccmr2.oc4pe = true;  // if true, the new value is actually loaded to the live reg on next update
      timer::get()->ccmr2.set(ccmr2.value);
   }

   {
     // set up TIM1_CH4 polarity high pulse
      tim::ccer_t ccer = timer::get()->ccer.get();
         ccer.cc4p = false; // TIM1_CH4 is positive pulse ( active high)
         ccer.cc4e = false; // dont enable TIM1_CH4 yet
      timer::get()->ccer.set(ccer.value);
   }

}

bool elevation_servo::enable()
{
    typedef azimuth_servo::timer timer;
    static constexpr uint8_t cc4e = 12U;  // ch 4 en
    timer::get()->ccer.bb_setbit<cc4e>();
    return true;
}

void elevation_servo::disable()
{
    typedef azimuth_servo::timer timer;
    static constexpr uint8_t cc4e = 12U;  // ch 4 en
    timer::get()->ccer.bb_clearbit<cc4e>();
}
