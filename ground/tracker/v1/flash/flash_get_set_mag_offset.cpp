/*
 Copyright (c) 2013 -2015 Andy Little

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

#include <quan/stm32/flash/flash_convert/vect3f.hpp>
#include <quan/stm32/flash/flash_convert/bool.hpp>

bool get_flash_use_compass (bool & value)
{
   return quan::stm32::flash::flash_convert<bool>::get("use_compass", value);
}
 
bool set_flash_use_compass (bool value)
{
   return quan::stm32::flash::flash_convert<bool>::set ("use_compass", value);
}
 
bool set_flash_mag_offset (quan::three_d::vect<float> const & value)
{
   return quan::stm32::flash::flash_convert<quan::three_d::vect<float> >::set ("mag_offsets", value);
}

bool get_flash_mag_offset (quan::three_d::vect<float> & value)
{
   return quan::stm32::flash::flash_convert<quan::three_d::vect<float> >::get ("mag_offsets", value);
}
