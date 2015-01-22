#ifndef QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
#define QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
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

#include <quan/three_d/vect.hpp>

// each type in flash needs a typeid
// these are zero based and are used as indexes
// in array
enum flash_type_tags { Vect3F, Bool};

// app level create unique integer tags for each type in the flash
// accessible using these templates
template <typename T> struct type_to_id;
template <uint32_t id> struct id_to_type;

template <> struct type_to_id<quan::three_d::vect<float> > {
   static constexpr uint32_t value = flash_type_tags::Vect3F;
};
template <> struct id_to_type<flash_type_tags::Vect3F>{
   typedef quan::three_d::vect<float> type;
};
//---------
template <> struct type_to_id<bool> {
   static constexpr uint32_t value = flash_type_tags::Bool;
};
template <> struct id_to_type<flash_type_tags::Bool>{
   typedef bool type;
};
//---

#endif // QUANTRACKER_FLASH_FLASH_TYPE_TAGS_HPP_INCLUDED
