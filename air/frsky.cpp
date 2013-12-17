/*
 Copyright (c) 2012 - 2013 Andy Little 

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
#include "settings.hpp"
#include "frsky.hpp"
#include "zapp2.hpp"
#include "serial_ports.hpp"
#include "events.hpp"

namespace {

   int16_t output (uint8_t const * buf, int16_t len)
   {
      ::frsky_sp::serial_port::write(reinterpret_cast<char const *>(buf),len);
      return len;
   }
}

namespace frsky{ namespace zapp2 {
   
   void send_message()
   {
      ::zapp2::send_message(output);
   }
}} // frsky::zapp2

namespace {

   periodic_event zapp2_event{
      ::zapp2::get_update_event_timing(),
      ::frsky::zapp2::send_message,true
   };
}

namespace frsky{ namespace zapp2{

   void setup_event()
   {
      ::set_event(::event_index::frsky,&zapp2_event);
   }

}}

namespace frsky{

   void setup()
   {
      frsky_sp::serial_port::init();
   //TODO : invert tx output according to eeprom
      frsky_sp::serial_port::set_baudrate<9600,false>();
   }

   void send_message()
   {
      switch (settings::frsky_protocol){
         case settings::output_protocol_t::zapp1:
         //   zapp1::frsky_send_message();
            break;
         case settings::output_protocol_t::zapp2:
            frsky::zapp2::send_message();
            break;
         default:
            break;
      }
   }

   void setup_event()
   {
      switch (settings::frsky_protocol){
         case settings::output_protocol_t::zapp1:
         //   zapp1::setup_frsky_event();
            break;
         case settings::output_protocol_t::zapp2:
            frsky::zapp2::setup_event();
            break;
         default:
            break;
      }
   }

}//frsky
