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
#if defined QUAN_STM32F4
#include <stm32f4xx.h>
#else
#if defined QUAN_STM32F0
#include <stm32f4xx.h>
#else
#error need to define processor
#endif
#endif
#include <cstdint>
#include <quan/time.hpp>
#include <quan/stm32/systick.hpp>

void do_event_ticks();

// periodic_event periodic_event
// actioned in some multiple of 1 ms
struct periodic_event{

   periodic_event(quan::time_<uint32_t>::ms const & period, void(*pf)(), bool enabled = false) :
   m_count{0U},m_target_count{period},m_pf{pf},
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
      quan::stm32::disable_SysTick_IRQ();
      m_target_count = period; m_pf = pf;
      quan::stm32::enable_SysTick_IRQ();
   }
   bool is_enabled() const {return m_enabled;}
   void enable(){ m_enabled = true;}
   void disable() { m_enabled = false;}
private:

   friend void do_event_ticks();
   void tick() 
   { 
      if (++m_count >= m_target_count){
         m_call_flag = true;
         m_count = quan::time_<uint32_t>::ms{0U};
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
   static constexpr uint32_t frsky = 0U;
   static constexpr uint32_t fsk = 1U;
   static constexpr uint32_t heartbeat = 2U;
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
