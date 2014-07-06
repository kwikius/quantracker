#ifndef FRSKY_DATA_FRSKY_SERIAL_PORT_HPP_INCLUDED
#define FRSKY_DATA_FRSKY_SERIAL_PORT_HPP_INCLUDED

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

#include <quan/uav/frsky2wsp/protocol.hpp>
//#include <quan/uav/cobs/protocol.hpp>
#include "serial_ports.hpp"

typedef  FrSky2WSP::protocol<sliprings::serial_port> frsky_serial_port;

// length is 1 byte for  message id, 4 for data, 1 for checksum = 6


#endif // FRSKY_DATA_FRSKY_SERIAL_PORT_HPP_INCLUDED
