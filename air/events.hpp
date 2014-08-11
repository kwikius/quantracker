#ifndef QUANTRACKER_AIR_EVENTS_HPP_INCLUDED
#define QUANTRACKER_AIR_EVENTS_HPP_INCLUDED
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

#include <stm32f0xx.h>
#include <cstdint>
#include <quan/time.hpp>

void do_event_ticks();

// periodic_event periodic_event
// actioned in some multiple of 1 ms
struct periodic_event{

   periodic_event(quan::time_<uint32_t>::ms const & period, void(*pf)(), bool enabled = false) :
   m_count{0},m_target_count{period},m_pf{pf},
   m_call_flag{false},m_enabled{enabled}{}

   void service() 
   { 
      if (m_enabled && m_call_flag){
         m_call_flag = false;
         m_pf();
      }
   }
   void set(quan::time_<uint32_t>::ms const & period, void(*pf)())
   {
      NVIC_DisableIRQ(SysTick_IRQn);
      m_target_count = period; m_pf = pf;
      NVIC_EnableIRQ(SysTick_IRQn);  
   }
   bool is_enabled() const {return m_enabled;}
   bool enable(){ m_enabled = true;}
   bool disable() { m_enabled = false;}
private:

   friend void do_event_ticks();
   void tick() 
   { 
      if (++m_count >= m_target_count){
         m_call_flag = true;
         m_count = quan::time_<uint32_t>::ms{0};
      }
   }
   quan::time_<uint32_t>::ms m_count;
   quan::time_<uint32_t>::ms m_target_count;
   void(*m_pf)();
   bool m_call_flag;
   bool m_enabled;
};

// indexes into the events array
struct event_index{
   static constexpr uint32_t frsky = 0;
   static constexpr uint32_t fsk = 1;
   static constexpr uint32_t heartbeat = 2;
};

periodic_event * get_event(uint32_t i);
void set_event(uint32_t i, periodic_event * ev);

void setup_events();
void setup_frsky_event();
void setup_fsk_event();
void setup_heartbeat_event();

void do_event_ticks();
void service_events();

#endif // QUANTRACKER_AIR_EVENTS_HPP_INCLUDED
