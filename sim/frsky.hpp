#ifndef QUANTRACKER_SIM_FRSKY_HPP_INCLUDED
#define QUANTRACKER_SIM_FRSKY_HPP_INCLUDED

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

//#include <quan/uav/position.hpp>
//#include <quan/angle.hpp>
//#include <quan/length.hpp>
#include "app.h"

#include <quan/serial_port.hpp>
#include <quan/utility/fifo.hpp>


   template < int Size>
   inline ssize_t send_as_frsky_Telemetry_from_RcTx( quan::fifo<uint8_t, Size> & buffer, quan::serial_port * sp)
   {
     ssize_t bytes_sent = 0;
     while ( buffer.num_in_buffer() > 0) { 
        uint8_t  msg_array[11] = {0x7E,0xFD,0,0,0,0,0,0,0,0,0x7E};
   //     
   //     msg_array[0] = 0x7E; // header
   //     msg_array[1] = 0xFD; // id user frame
   //     msg_array[2] = 0;   // number of user bytes
   //     msg_array[3] = 0;    // unused
   //     for( uint32_t i = 0; i < 6; ++i){ // pre clear user data
   //        msg_array[4+i] = 0;
   //     }
   //     msg_array[10] = 0x7E; // end of frame
        uint8_t * const num_user_bytes = &msg_array[2];
        uint8_t * p_bytes = &msg_array[4];
        while( buffer.num_in_buffer() > 0){
          uint8_t ch;
          buffer.peek(ch);
          if ( (ch == 0x7E) || (ch == 0x7D)){
            if ( *num_user_bytes < 5){
                buffer.get(ch);
                *p_bytes++ = 0x7D;
                *p_bytes++ = ch ^ 0x20;
                *num_user_bytes +=2;
            }
            else{
               break;
            }
          }else{
             if (*num_user_bytes < 6){
               buffer.get(ch);
               *p_bytes++ = ch;
               ++ *num_user_bytes;
             }else{
                break;
             }
          }
        }
        bytes_sent += sp->write(msg_array,11);
     }
     return bytes_sent;
   }

#endif //QUANTRACKER_SIM_FRSKY_HPP_INCLUDEDi
