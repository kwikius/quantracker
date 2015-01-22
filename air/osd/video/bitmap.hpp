#ifndef OSD_BITMAP_HPP_INCLUDED
#define OSD_BITMAP_HPP_INCLUDED

#include <cstdint>
#include <quan/two_d/vect.hpp>
 
/*
 Copyright (c) 2013-2015 Andy Little 

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

template <typename DataType>
struct abc_bitmap {
   virtual quan::two_d::vect<uint32_t> get_size_px() const =0;
   virtual uint32_t get_bits_px() const=0;
   virtual DataType const * get_data() const=0;
   uint32_t get_data_length()
   {
      quan::two_d::vect<uint32_t> const sz = get_size_px();
      return sz.x * sz.y * get_bits_px();
   }
   virtual ~abc_bitmap() {}
};
 
template <typename DataType, uint32_t X_size, uint32_t Y_size,uint32_t Bits_px>
struct static_image : abc_bitmap<DataType> {
   quan::two_d::vect<uint32_t> get_size_px() const {
      return {X_size,Y_size};
   }
   uint32_t get_bits_px() const {
      return Bits_px;
   }
};

#endif // OSD_BITMAP_HPP_INCLUDED
