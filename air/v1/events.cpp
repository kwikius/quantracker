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

#include "events.hpp"

namespace zapp1{
   void frsky_send_message();
}

namespace{
   periodic_event* events[3] = {nullptr, nullptr,nullptr};
}

void set_event(uint32_t i, periodic_event * ev)
{
   events[i] = ev;
}

// called by systick handler
void do_event_ticks()
{
   for(auto e : events){
      if (e && e->is_enabled()){
         e->tick();
      }
   }
}

// called by main loop
void service_events()
{
   for(auto e : events){
      if( e && e->is_enabled()){
         e->service();
      }
   }
}
