/*
 Copyright (c) 2003-2015 Andy Little.

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
#include "settings.hpp"

#if 0 
/* change BOOT1 option to allow use of serial port to program.
If one is written then all must be rewritten
*/
#if defined QUAN_STM32F0
extern uint32_t const user_options1 __attribute__ ((section ("processor_options1")))  = 0x10EF55AA;
extern uint32_t const user_options2 __attribute__ ((section ("processor_options2")))  = 0x00FF00FF;
extern uint32_t const user_options3 __attribute__ ((section ("processor_options3")))  = 0x00FF00FF;
#endif
#endif

settings::altitude_t settings::altitude_src 
   = settings::altitude_t::baro_alt;

settings::output_protocol_t settings::frsky_protocol 
   = settings::output_protocol_t::zapp1;

settings::output_protocol_t settings::fsk_protocol 
   = settings::output_protocol_t::zapp3;

settings::data_source_t settings::data_source
   = settings::data_source_t::mavlink;
