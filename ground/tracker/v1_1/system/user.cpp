#include "resources.hpp"
#include <quan/user.hpp>

namespace {
    typedef gcs_serial sp;
}

void quan::user_message (const char* text)
{
    sp::write(text);
}

//not blocking
uint32_t quan::user_in_avail() 
{ 
   return sp::in_avail();
}

char quan::user_get() 
{ 
   return sp::get(); 
}

void quan::user_flush_sptx()
{
   while (sp::out_waiting() > 0U ){asm volatile("nop":::);}
}
