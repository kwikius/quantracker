#include "resources.hpp"
#include <quan/user.hpp>

/*
 Copyright (c) 2017 Andy Little 

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
