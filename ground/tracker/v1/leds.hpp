#ifndef QUANTRACKER_LEDS_HPP_INCLUDED
#define QUANTRACKER_LEDS_HPP_INCLUDED

/*
 Copyright (c) 2013 Andy Little 

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

#include "resources.hpp"
#include <quan/time.hpp>

template <typename Pin>
struct Led{
   Led(){}

   typedef Pin pin_type;

   void switch_on()
   { 
      m_on_period = quan::time_<int32_t>::ms{1};
      m_off_period = quan::time_<int32_t>::ms{0};
      quan::stm32::set<Pin>();
   }
   void switch_off()
   { 
      m_off_period = quan::time_<int32_t>::ms{1};
      m_on_period = quan::time_<int32_t>::ms{0};
      quan::stm32::clear<Pin>();
   }
   void set_flashing(
         quan::time_<int32_t>::ms on_period, 
         quan::time_<int32_t>::ms off_period
   ){
      m_count = m_on_period = on_period;
      m_off_period = off_period;
      quan::stm32::set<Pin>();
   }
   bool is_flashing() const
   {
      return ( m_on_period > quan::time_<int32_t>::ms{0} ) 
            && (m_off_period > quan::time_<int32_t>::ms{0});
   }

   // in 20 ms event
   void update()
   {
      if ( is_flashing() ){ 
         m_count -= quan::time_<int32_t>::ms{50};
         if ( m_count <= quan::time_<int32_t>::ms{0}){
            if (quan::stm32::get<Pin>() == true){
               m_count = m_off_period;
               quan::stm32::clear<Pin>();
            }else{
               m_count = m_on_period;
               quan::stm32::set<Pin>();
            }
         }
      }
   }
private:
   quan::time_<int32_t>::ms m_count;
   quan::time_<int32_t>::ms m_on_period;
   quan::time_<int32_t>::ms m_off_period;
   void flash_update();
};

extern Led<green_led_pin>     tracking_led;
extern Led<orange_led_pin>       error_led;
extern Led<blue_led_pin>      startup_led;
extern Led<red_led_pin>    heartbeat_led;

void setup_leds();
void update_leds();

#endif // QUANTRACKER_LEDS_HPP_INCLUDED
