#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

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

//#define GPS_DEBUG_OUTPUT

#define QUAN_TELEMETRY_ALTITUDE_GPS 1
#define QUAN_TELEMETRY_ALTITUDE_BARO 2

// hack so I dont have to redo tracker code for now.
// will prob move to cobs anyway..

#define QUAN_TELEMETRY_RETURN_GPS_ALTITUDE_AS_BARO_ALTITUDE 3

#define QUAN_TELEMETRY_ALTITUDE_TYPE QUAN_TELEMETRY_RETURN_GPS_ALTITUDE_AS_BARO_ALTITUDE
//#define QUAN_TELEMETRY_ALTITUDE_TYPE QUAN_TELEMETRY_ALTITUDE_BARO
#endif // CONFIG_H_INCLUDED
