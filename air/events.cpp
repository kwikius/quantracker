
#include "events.hpp"

namespace zapp1{
   void frsky_send_message();
}

namespace{
   periodic_event* events[3] = {nullptr, nullptr,nullptr};
}
#if 0
periodic_event * get_event(uint32_t i)
{
   return events[i];
}
#endif

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
