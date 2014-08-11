
#ifndef QUANTRACKER_SWITCH_INPUT_HPP_INCLUDED
#define QUANTRACKER_SWITCH_INPUT_HPP_INCLUDED
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

#include <quan/time.hpp>
#include "resources.hpp"
#include "tracker_states.hpp"

// all switches have the same debounce time
struct basic_switch_input{
   static quan::time_<int32_t>::us m_max_debounce_time;
};

void switches_update();

template <typename State>
struct switch_input;


// simpler for bool states
template <>
struct switch_input<bool> : basic_switch_input{
   typedef void (*callback_t)(bool);
   switch_input(bool state,callback_t callback)
   : m_state{state},m_debounce_time{0}, m_callback{callback}
   {}

   void force_state(bool state)
   {
     // if we forced a change of state then must do the callback
     bool old_state = m_state;
   
     m_state = state;
     m_debounce_time = quan::time_<int32_t>::us{0};
     if ( (old_state != state) && ( m_callback != nullptr)){
         m_callback(m_state);
     }
   }
   void update_state(quan::time_<int32_t>::us dt)
   {
      bool cur_state = get_instant_state();
      if( cur_state == m_state ){
         m_debounce_time = quan::time_<int32_t>::us{0};
      }else{
         m_debounce_time += dt;
         if ( m_debounce_time >= m_max_debounce_time){
            m_state = cur_state;
            m_debounce_time = quan::time_<int32_t>::us{0};
            if ( m_callback != nullptr){
               m_callback(m_state);
            }
         }
      }
  }
  bool get_state() const{ return m_state;}
protected:
   virtual bool get_instant_state()const =0;
private:
   bool m_state;
   quan::time_<int32_t>::us m_debounce_time;
   callback_t m_callback;
};

// true is on is pressed
template <typename Pin>
struct input_button : switch_input<bool>
{
    bool get_instant_state() const { return quan::stm32::get<Pin>();}
    input_button(typename switch_input<bool>::callback_t callback) : switch_input{false,callback}{}
};

extern input_button<user_switch_pin> user_button;


void setup_switches();

#endif //QUANTRACKER_SWITCH_INPUT_HPP_INCLUDED
