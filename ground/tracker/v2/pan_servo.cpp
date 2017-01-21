
#include <stm32f4xx.h>
#include <FreeRTOS.h>
#include <task.h>

#include <quan/max.hpp>
#include <quan/stm32/sys_freq.hpp>
#include <quan/angle.hpp>
#include <quan/reciprocal_time.hpp>
#include <quan/frequency.hpp>
#include <quan/voltage.hpp>
#include <quan/current.hpp>
#include <quan/time.hpp>
#include <quan/meta/integer_max.hpp>
#include <quan/stm32/get_raw_timer_frequency.hpp>
#include <quan/stm32/push_pop_fp.hpp>
#include "resources/tracker_resources.hpp"
#include "resources/osd_resources.hpp"
#include "tracker_commands.hpp"
/*
   pan servo
   With current test rig
   Clockwise rotation of the spindle gives a positive back emf
   setting Dir = 1 gives positive rotation
   Interrupts are chained
   Since they are low priority, ensure that there is a minimum delay
   (say 20 us) so that interrupts always serviced
    Need to sort a  way to recover if interrupts missed .. watchdog?

    Change so that Uif is set to period longer than max of pwm period
    Use compare to do chained interrupts and use uif to reset to start if
    no compare before overflow. Uif should provide a warning if tripped
*/

/*
   Should be possible to do better
   At start of cycle
      1)setup for a current ADC conversion triggered by timer set to the on duty cycle and enable EOC irq.

      2) in the EOC irq, turn off the motor and calc the time to start emf sampling
      setup for emf ADC conversions in a dma sequence.
      setup the trigger time to the first conversion but setup timer dma to fire once to
      modify the period (TIMx_ARR) once first conv is underway to the period for repeated emf ADC conversion
      and setup the ADC DMA to the number of conversions

      3) In emf conversions DMA complete irq, calc pwm etc and restart the cycle

     ADC can be triggered by various timer channels  of TIM1, TIM2,TIM3, TIM4, TIM5, TIM8
      see ref_man section 10.6 table 35  
*/

namespace {

   // Potentially useful output units
   // assuming we want to track in radians per sec
   // rad is the value_type.. should be be used like e.g float
   // pedantic use of units here. could use e.g float but got radians type so may as well use it!
   typedef quan::angle::rad rad;
   typedef quan::reciprocal_time_<rad>::per_s rad_per_s;

   // this value updated by the external tracker logic
   // The servo is servoing on this value
   rad_per_s target_output_angular_velocity{rad{0.f}};

   // the angular velocity we want the tracker pan output to be at 
   // nominal 1/50th sec pan_motor_pwm_period
   constexpr quan::time::us pan_motor_pwm_period{20000.f};

   // using LD39015 3.3V reg voltage output accuracy quoted as +- 2%
   // 
   constexpr quan::voltage::mV volts_per_bit = quan::voltage::mV{3300.f}/4096.f;

 // ################ flash variables ############
   // default to 1 rev per sec with 12v input
   // needs to be measured for a particular setup
   auto constexpr output_angular_velocity_per_mV 
   = rad_per_s{quan::angle::two_pi} 
         / quan::voltage::mV{12000};

   // proportional constant tried with kP =0.3 and kD = 0.6 on small motor
   // works ok
   auto  kP = 0.1f/rad_per_s{rad{1.f}};
   // differential constant is always 2 x proportional
   auto  kD = 0.2f/rad_per_s{rad{1.f}};
   // 0.1 gives a minimum on pulse of 2 ms 
   // needs to be big enough to move the motor rateher
   // than just causing buzzing
   float min_duty_cycle = 0.05f;
   // constant for current dependent delay
   // after switching off motor
   // set here to a spike of 2 ms per 1 A current
   auto constexpr current_spike_constant = quan::time::us{2000.f}/quan::current::mA{1000.f};

   //####### end flash variables ###############

   // where the zero volts rail is should be approx half of supply
   // actual value found at  startup by reading the 
   // motor with no input until it is in some range
   // e.g so not being moved by user
   quan::voltage::mV emf_zero_volts_rail{1650.f};
   // If using  ACS712 current sensor also need a  zero volts rail
   // value found at startup
   quan::voltage::mV current_zero_volts_rail{1650.f};

   // duty cycle as fraction of period somewhere between -1 and 1
   // (accumulated)
   float duty_cycle = 0.f;
   // std::abs(duty_cycle) max limit
   // need some velocity samples!
    float max_duty_cycle = 0.75f;

   // velocity error from prev iter
   rad_per_s old_velocity_error{rad{0.f}};

   bool pan_motor_enabled = false;

   // keeps a tally of how much of cycle used by each section
   quan::time::us cycle_time_left = pan_motor_pwm_period;
   void section_start_new_cycle();
   // set initial interrupt
   void (*pf_timer_irq_section_function)() = &section_start_new_cycle;
   
   void set_next_timer_irq(quan::time::us t, void(*pfn)())
   {
     pf_timer_irq_section_function = pfn;
     int32_t time_to_irq = static_cast<int32_t>(t.numeric_value());
     if ( time_to_irq < 0){
         time_to_irq = 10;
     }
     pan_motor_timer::get()->arr = time_to_irq;
     cycle_time_left -= t;
     pan_motor_timer::get()->cnt = 0U;
     quan::stm32::enable<pan_motor_timer>();
   }
  
   void section_switch_on_motor();
   void section_start_backemf_sampling();

   // accumulator for velocity( back_emf) sampling
   quan::voltage::mV back_emf_sum{0};
   // used to record number of velocity samples this cycle
   uint32_t num_backemf_samples = 0U;

   // recalced each cycles
   quan::voltage::mV motor_back_emf{0};
   quan::current::mA motor_current{0};
   // the pid loop on velocity
  // int led_counter = 0;
   void section_start_new_cycle()
   {
//      ++led_counter;
//      if (led_counter == 50){
//         led_counter = 0;
//        // quan::stm32::complement<heartbeat_led_pin>();
//      }
      // reinvigorate for new cycle
      cycle_time_left = pan_motor_pwm_period;
      // get motor angular velocity by averaging samples from last cycle
      // but avoid divison by zero
      motor_back_emf =
      ( num_backemf_samples > 0)
         ? (back_emf_sum / num_backemf_samples) - emf_zero_volts_rail
         : quan::voltage::mV{0};
      // constrain to a min
      if (quan::abs(motor_back_emf) < quan::voltage::mV{5}){
         motor_back_emf = quan::voltage::mV{0};
      }
      rad_per_s const actual_output_velocity = motor_back_emf * output_angular_velocity_per_mV;
      // get term proportional to velocity error
      rad_per_s const velocity_error = target_output_angular_velocity - actual_output_velocity;
      duty_cycle += kP * velocity_error;
      // get term proportional to rate of change of velocity error
      rad_per_s const velocity_error_de_dt = velocity_error - old_velocity_error;
      old_velocity_error = velocity_error;
      duty_cycle += kD * velocity_error_de_dt;

     // duty_cycle = target_output_angular_velocity.numeric_value();
      auto const abs_duty_cycle = std::fabs(duty_cycle);
      if ( abs_duty_cycle < 0.01f){
         duty_cycle = 0.f;
      }
     // static_assert(std::is_same<decltype(abs_duty_cycle),float const>::value,"odd");
      if ( abs_duty_cycle > min_duty_cycle){
         if ( abs_duty_cycle > max_duty_cycle){
            int const sign_duty_cycle = (duty_cycle > 0.f)?1:-1;
            duty_cycle = max_duty_cycle * sign_duty_cycle;
         }
         section_switch_on_motor();
      }else{ // duty_cycle too small so dont switch on motor
         section_start_backemf_sampling();
      }
   }

   void set_motor_direction(int direction)
   {
      if (direction >= 0){
         quan::stm32::set<pan_motor_dir_pin>();
      }else{
         quan::stm32::clear<pan_motor_dir_pin>();
      }
     
   }

   void switch_on_motor()
   {
     // quan::stm32::set<heartbeat_led_pin>();
      quan::stm32::set<pan_motor_pwm_pin>();
   }

   void switch_off_motor()
   {
     // quan::stm32::clear<heartbeat_led_pin>();
      quan::stm32::clear<pan_motor_pwm_pin>();
   }

   void section_pre_end_duty_cycle();

   void section_switch_on_motor2();
   void section_switch_on_motor()
   {
    
      int const direction = ((duty_cycle >= 0.f) ? 1 : -1);
      set_motor_direction(direction);
       // on L298 give ample time for direction logic output to stabilise
       // before switching motor on
      set_next_timer_irq(quan::time::us{20.f}, &section_switch_on_motor2);
   }
   // Now really do switch motor on!
   void section_switch_on_motor2()
   {
      if (pan_motor_enabled){
         switch_on_motor();
      }
      set_next_timer_irq(std::fabs(duty_cycle) * pan_motor_pwm_period, &section_pre_end_duty_cycle);
   }
   
   void start_pan_motor_current_conversion()
   {
       // select motor current ADC channel
       ADC1->SQR3 = 11;
       // start a conversion this will cause an ADC irq
       ADC1->CR2 |= (1 << 30);  // (SWSTART) 
   }

   // called before switching motor off to sample the current
   void section_pre_end_duty_cycle()
   {
      start_pan_motor_current_conversion();
   }

   // returns raw voltage with respect to GND
   quan::voltage::mV read_a2d_voltage()
   {
      return volts_per_bit * ((uint32_t)ADC1->DR) ;
   }

   // returns signed current
   quan::current::mA read_a2d_current()
   {
      constexpr auto amps_volt = quan::current::mA{5000} / quan::voltage::mV{1650};
      auto const voltage_reading = read_a2d_voltage() - current_zero_volts_rail;
      return voltage_reading * amps_volt;
   }

   // called by ADC current interrupt
   void section_switch_off_motor()
   {
      // dont care about current direction
      motor_current = read_a2d_current();
      quan::current::mA const current = quan::abs(motor_current);
      switch_off_motor();
      quan::time::us const spike_time = quan::max(current * current_spike_constant, quan::time::us{1000});
      set_next_timer_irq(spike_time,&section_start_backemf_sampling);
   }

   uint32_t num_backemf_samples_left = 0U;
   uint32_t constexpr backemf_samples_per_full_cycle = 20U;

   auto constexpr time_between_backemf_samples = pan_motor_pwm_period / backemf_samples_per_full_cycle;

   void start_backemf_conversion()
   {
      ADC1->SQR3 = 10;
      // causes ADC interrupt on complete
      ADC1->CR2 |= (1 << 30);  // (SWSTART)
   }
/*
   should use DMA here
   set up dma timer 
*/
   void section_start_backemf_sampling()
   {
      
      back_emf_sum = quan::voltage::mV{0};
      int32_t const num_samples 
         = static_cast<int32_t>(cycle_time_left / time_between_backemf_samples);
       
      if ( num_samples > 0){
         num_backemf_samples = num_samples;
      }else{
         num_backemf_samples = 1;
      }
      num_backemf_samples_left = num_backemf_samples;
      start_backemf_conversion();
   }

   // called from ADC irq
   void section_get_velocity()
   {
      
      back_emf_sum += read_a2d_voltage();
      if ( --num_backemf_samples_left > 0){
         set_next_timer_irq(time_between_backemf_samples,&start_backemf_conversion);
      }else{
         section_start_new_cycle();
      }
   }

} // namespace

using namespace quan::stm32;

namespace {
   void setup_pins()
   {
      module_enable<pan_motor_pwm_pin::port_type>();
      module_enable<pan_motor_dir_pin::port_type>();

      apply<
         pan_motor_pwm_pin
         ,gpio::mode::output     
         ,gpio::otype::push_pull  
         ,gpio::ospeed::slow 
         ,gpio::ostate::low
      >();
      // direction = 1
      apply<
         pan_motor_dir_pin
         ,gpio::mode::output
         ,gpio::otype::push_pull
         ,gpio::ospeed::slow
         ,gpio::ostate::high
      >();

      // analog pins
      module_enable<pan_motor_emf_adc_pin::port_type>();
      module_enable<pan_motor_current_adc_pin::port_type>();

      apply<
         pan_motor_emf_adc_pin      
         ,gpio::mode::analog
         ,gpio::pupd::none
      >();

      apply<
         pan_motor_current_adc_pin
         ,gpio::mode::analog
         ,gpio::pupd::none
      >();
   }

   // todo add to quan-trunk
   template <typename Timer,typename Time>
   inline 
   bool set_timer_clock(Time const & clock_time){
      constexpr quan::frequency::Hz raw_freq{quan::stm32::get_raw_timer_frequency<Timer>()};
      int32_t const prescaler_value = static_cast<uint32_t>((raw_freq * clock_time) + 0.5f)-1;
      if (( prescaler_value >= 0) && (prescaler_value <= quan::meta::integer_max<uint16_t>::value)){
         pan_motor_timer::get()->psc = prescaler_value;
         return true;
      }else{
         return false;
      }
   }
    
   // todo add to quan-trunk
   template <typename Timer>
   inline 
   void set_one_shot_mode()
   {
      Timer::get()->cr1. template bb_setbit<3>(); // (OPM)
   }

   template <typename Timer>
   inline void enable_overflow_interrupt()
   {
     Timer::get()->dier. template bb_setbit<0>(); // (UIE)
   }

   template <typename Timer>
   inline void disable_overflow_interrupt()
   {
     Timer::get()->dier. template bb_clearbit<0>(); // (UIE)
   }

   void setup_timer()
   {
      module_enable<pan_motor_timer>();
      set_timer_clock<pan_motor_timer>(quan::time::us{1});
      set_one_shot_mode<pan_motor_timer>();
      // interrupt will be enabled in 
      NVIC_SetPriority(TIM8_UP_TIM13_IRQn,local_interrupt_priority::pan_motor);
      NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
   }

   // uses ADC1
   void setup_voltage_and_current_adcs()
   { // voltage on pc0  ADC123_IN10, 
      // current on pc1 ADC123_IN11
      // turn on ADC1 module
      // right aligned is default
      quan::stm32::rcc::get()->apb2enr.bb_setbit<8>(); //( ADC1)
      // reset ADC1
      quan::stm32::rcc::get()->apb2rstr.bb_setbit<8>(); // (ADC1)
      quan::stm32::rcc::get()->apb2rstr.bb_clearbit<8>();
      // set ADC prescale not > 21 MHz
      ADC->CCR |= (0b11 << 16); // 8 Cyc if APB2 == sysclk
      //ADC->CCR |= (0b01 << 16);   // 4 Cyc if APB2 == syclk / 2
      //set up sample time for channels
      // to 0b100 == 84 cycles == 4 usec
      ADC1->SMPR1 |= (0b100 << 3) | 0b100  ; // (SMP11 5:3) (SMP10 2:0)
      ADC1->CR1  |= (1 << 5) ; // (EOCIE)
      NVIC_SetPriority(ADC_IRQn,local_interrupt_priority::pan_motor);
      NVIC_EnableIRQ (ADC_IRQn);
      ADC1->CR2 |= (1 << 0) ;  //(ADON)
      //wait 2 - 3 usecs after ADON for startup
      uint32_t wait = 0;
      uint32_t constexpr done = quan::stm32::get_sysclk_frequency()/1000000U;
      while(wait < done){
         ++ wait;
         asm volatile("nop":::);
      }
   }

   // assumes right aligned adc,ADON == 1
   quan::voltage::mV do_synchronous_conversion()
   {
       // save state
       auto const adc_cr1 = ADC1->CR1;
       auto const adc_cr2 = ADC1->CR2;
       ADC1->CR1 = 0;
       ADC1->CR2 = 1U; // leave only (ADON)
       ADC1->CR2 |= (1 << 30);  // (SWSTART)
       // just poll as interrupt is off
       while ( !(ADC1->SR & (1 << 1) ) ){;} // (EOC)
       auto const result = volts_per_bit * ((uint32_t)ADC1->DR) ;
       // EOC cleared by read
       // restore state
       ADC1->CR1 = adc_cr1 ;
       ADC1->CR2 = adc_cr2 ;
       return result;
   }

} // namespace

quan::voltage::mV tracker::pan::get_back_emf_0v_rail()
{
   return emf_zero_volts_rail;
}

quan::voltage::mV tracker::pan::get_current_0v_rail()
{
   return current_zero_volts_rail;
}

void tracker::pan::enable(bool b)
{
   
   if (b){
      duty_cycle = 0.f;
      motor_back_emf = quan::voltage::mV{0};
      motor_current = quan::current::mA{0};
      pf_timer_irq_section_function = &section_start_new_cycle;
      pan_motor_timer::get()->sr = 0;
      pan_motor_timer::get()->arr = 100;
      pan_motor_timer::get()->cnt = 0;
      old_velocity_error = rad_per_s{rad{0}};
      pan_motor_enabled = true;
      quan::stm32::enable<pan_motor_timer>();
      enable_overflow_interrupt<pan_motor_timer>();
   }else{
      disable_overflow_interrupt<pan_motor_timer>();
      pan_motor_enabled = false;
      switch_off_motor();
      quan::stm32::disable<pan_motor_timer>();
   }
}

bool tracker::pan::enabled()
{
   return pan_motor_enabled;
}

void tracker::pan::set_angular_velocity(tracker::rad_per_s const & v)
{
   target_output_angular_velocity = v;
}

rad_per_s tracker::pan::get_angular_velocity()
{
  return motor_back_emf * output_angular_velocity_per_mV;
}

rad_per_s tracker::pan::get_target_angular_velocity()
{
  return target_output_angular_velocity;
}

void tracker::pan::set_kP(float val)
{
  kP = val / rad_per_s{rad{1.f}};
}

void tracker::pan::set_kD(float val)
{
  kD = val / rad_per_s{rad{1.f}};
}

void tracker::pan::set_kC(float val)
{
  min_duty_cycle = val ;
}

namespace tracker_detail{

   void set_pan_motor_zero_rails()
   {
      quan::voltage_<double>::mV average_voltage_zero{0};
      quan::voltage_<double>::mV average_current_zero{0};
      for (uint32_t i =0; i < 4096;++i){
         ADC1->SQR3 = 10;
         average_voltage_zero += do_synchronous_conversion();
         ADC1->SQR3 = 11;
         average_current_zero += do_synchronous_conversion();
      } // ~for
      emf_zero_volts_rail = static_cast<quan::voltage::mV>(average_voltage_zero/4096.0);
      current_zero_volts_rail = static_cast<quan::voltage::mV>(average_current_zero/4096.0);
   }

   void pan_motor_setup()
   {
        
        setup_pins();
        setup_timer();
        setup_voltage_and_current_adcs();
        set_pan_motor_zero_rails();
   }
}

/*
 nb need to push fp regs in all these interrupts
*/
extern "C" void ADC_IRQHandler() __attribute__ ( (interrupt ("IRQ")));

extern "C" void ADC_IRQHandler()
{
  if ( ADC1->SR & ( 1 << 1) ) { // (EOC)
      // (EOC cleared by read)
     // quan::stm32::set<heartbeat_led_pin>();
      quan::stm32::push_FPregs();
      if (ADC1->SQR3 == 10){
         section_get_velocity();
      }else{
         section_switch_off_motor();
      } 
      quan::stm32::pop_FPregs();
  }
  // set heartbeat led
  // otherwise clear irq?
}
namespace {
 int count = 0;
}
extern "C" void TIM8_UP_TIM13_IRQHandler() __attribute__ ( (interrupt ("IRQ")));

extern "C" void TIM8_UP_TIM13_IRQHandler()
{
    
    if ( pan_motor_timer::get()->sr.bb_getbit<0>()){ // (UIF)
      pan_motor_timer::get()->sr.bb_clearbit<0>(); // (UIF)
      quan::stm32::push_FPregs();
      if ( ++count == 50){
         count = 0;
         quan::stm32::complement<heartbeat_led_pin>();
      }
      pf_timer_irq_section_function();
      quan::stm32::pop_FPregs();
    }
    // otherwise its a TIM8 irq ?
    // if its a tim13 irq
    // clear flag and call current fn
}