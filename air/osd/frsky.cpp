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

#include "serial_ports.hpp"
#include "events.hpp"

namespace {

   int16_t output (uint8_t const * buf, int16_t len)
   {
      ::frsky_sp::serial_port::write(reinterpret_cast<char const *>(buf),len);
      return len;
   }
   //need to invert output for FrSky
  // but not for eg OpenLRS
   bool invert_frsky_tx = true;
}

namespace frsky{

   void setup()
   {
      frsky_sp::serial_port::init();
   //TODO : invert tx output according to eeprom
   // cant invert on stm32f4 may need external xor gate
      if ( invert_frsky_tx == true){
         typedef frsky_sp::serial_port::usart_type usart;
         static constexpr uint8_t txinv_bit = 17;
         bool const enabled = frsky_sp::serial_port::is_enabled();
         if (enabled){
          quan::stm32::disable<usart>();
         }
         usart::get()->cr2.setbit<txinv_bit>();
         if(enabled){
            quan::stm32::enable<usart>();
         }
      }
      frsky_sp::serial_port::set_baudrate<9600,false>();
      
   }

   void send_message()
   {
      zapp1::frsky_send_message();
   }

   void setup_event()
   {
      zapp1::setup_frsky_event();
   }

}//frsky
