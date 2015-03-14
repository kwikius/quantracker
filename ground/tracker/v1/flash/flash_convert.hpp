#ifndef QUANTRACKER_FLASH_CONVERT_HPP_INCLUDED
#define QUANTRACKER_FLASH_CONVERT_HPP_INCLUDED

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

#include <cstdint>
#include <quan/dynarray.hpp>
#include <quan/three_d/vect.hpp>

template <typename T>
struct flash_convert;

/*
 bytestream is the representation of the object in flash
 text is the user comprehensible string representing the object
*/

template <> struct flash_convert<bool>{
   static bool text_to_bytestream(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
   static bool bytestream_to_text(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
};

template <> struct flash_convert<quan::three_d::vect<float> >{
   static bool text_to_bytestream(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src);
   static bool bytestream_to_text(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src);
};

#endif
