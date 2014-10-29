
#include <quan/meta/type_sequence.hpp>
#include <quan/meta/for_each.hpp>
#include "../resources.hpp"

namespace {
      
   struct do_led_pin_setup
   {
      template <typename Pin>
      void operator()()const
      {
         quan::stm32::module_enable< typename Pin::port_type>();

         quan::stm32::apply<
            Pin
            , quan::stm32::gpio::mode::output
            , quan::stm32::gpio::otype::push_pull
            , quan::stm32::gpio::pupd::none
            , quan::stm32::gpio::ospeed::slow
            , quan::stm32::gpio::ostate::low
         >();
      }
   };
}

namespace  quan{ namespace impl{
   template<> struct is_model_of_impl<quan::meta::PolymorphicFunctor<1,0>,do_led_pin_setup > : quan::meta::true_{};
}}


void setup_leds()
{
   typedef quan::meta::type_sequence<
      red_led_pin
      ,blue_led_pin     
      ,green_led_pin  
      ,orange_led_pin 
   > led_pins;
  
   quan::meta::for_each<led_pins,do_led_pin_setup>{}();

}