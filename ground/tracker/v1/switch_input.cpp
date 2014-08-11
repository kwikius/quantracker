/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */
#include "switch_input.hpp"
#include "events.hpp"
#include <stm32f4xx.h>
#include <quan/stm32/systick.hpp>

quan::time_<int32_t>::us 
basic_switch_input::m_max_debounce_time{50000};// 1/20th sec


input_button<user_switch_pin> user_button{on_change_button_state};

void switches_update()
{
   constexpr auto dt = quan::time_<int32_t>::ms{1};
   user_button.update_state(dt);
}

using namespace quan::stm32;


namespace {
      
   struct do_switch_setup
   {
      template <typename Pin>
      void operator()()const
      {
         module_enable< typename Pin::port_type>();

         apply<
            Pin
            , gpio::mode::input
            , gpio::pupd::pull_down
         >();
      }
   };
}

namespace  quan{ namespace impl{
   template<> struct is_model_of_impl<quan::meta::PolymorphicFunctor<1,0>,do_switch_setup > : quan::meta::true_{};
}}

void setup_switches()
{
  
   do_switch_setup{}.operator()<user_switch_pin>();

}







