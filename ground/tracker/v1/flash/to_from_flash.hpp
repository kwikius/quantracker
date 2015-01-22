#ifndef QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
#define QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED

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

#include <quan/dynarray.hpp>
#include <cstdint>

bool get_flash_mag_offset(quan::three_d::vect<float> & dest);
bool set_flash_mag_offset(quan::three_d::vect<float> const & src);

bool get_flash_use_compass(bool & dest);
bool set_flash_use_compass(bool const & src);

#endif // QUAN_CPP_PROJECT_TEST_CONV_FUNCS_HPP_INCLUDED
