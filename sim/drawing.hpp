#ifndef QUANTRACKER_SIM_DRAWING_HPP_INCLUDED
#define QUANTRACKER_SIM_DRAWING_HPP_INCLUDED
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

#include <quan/gx/drawing.hpp>
#include <quan/length.hpp>

struct drawing : quan::gx::drawing<quan::length::mm>{
   typedef quan::length::mm mm;;
   using quan::gx::drawing<mm>::vect;
   drawing(){}
   vect get_size() const;
   vect get_origin() const {return vect{mm{0},mm{0}};}
};


#endif
