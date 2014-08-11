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
#include <cstdint>
#include <cassert>
#include <cstring>

#include <quan/tracker/zapp3_encode.hpp>
#include "aircraft.hpp"
#include "app.h"
 
// for 1/5 sec refresh rate
void COBS_send_message()
{
   uint8_t encoded[16];
   quan::tracker::zapp3_encode (get_aircraft_gps_position(),encoded);
   
   auto & app =wxGetApp();
   assert (app.have_sp());
   auto sp = app.get_sp();
   sp->write (encoded,16);
}
 
 
 
 
 
