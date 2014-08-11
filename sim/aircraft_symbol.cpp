/*
 Copyright (c) 2003-2014 Andy Little.

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
#include "aircraft_symbol.hpp"
    
quan::two_d::vect<quan::length::mm> 
aircraft_symbol::m_array[aircraft_symbol::num_points] =  
{
   {mm{0},mm{0 + 230}},
   {mm{200},mm{-165 + 230}},
   {mm{600},mm{-410 + 230}},
   {mm{600},mm{-560 + 230}},
   {mm{200},mm{-395 + 230}},
   {mm{0},mm{-450 + 230}},
   {mm{-200},mm{-395 + 230}},
   {mm{-600},mm{-560 + 230}},
   {mm{-600},mm{-410 + 230}},
   {mm{-200},mm{-165 + 230}},
   {mm{0},mm{0 + 230}}
  
};

