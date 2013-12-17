
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
#include <cstdint>

#include "mavlink.hpp"
#include "gps.hpp"
#include "settings.hpp"
#include "events.hpp"

extern "C" void setup();

namespace {

   void read_gps()
   { 
      the_gps.parse();
   }

   void read_data()
   {
      switch( settings::data_source){
         case settings::data_source_t::mavlink:
            read_mavlink();
         break;
         default:
            read_gps();
         break;
      }
   }
}

int main()
{
   setup();
   for(;;){
      read_data();
      service_events();
   }
}
