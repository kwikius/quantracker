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
#include <cstring>
#include <cstdio>
#include <quan/conversion/float_convert.hpp>
#include <quan/conversion/float_to_ascii.hpp>
#include <quan/dynarray.hpp>
#include <quan/three_d/vect.hpp>
#include "flash.hpp"
#include "flash_error.hpp"
#include "flash_convert.hpp"

bool set_flash_use_compass(bool val)
{
   int32_t const sym_index = flash_symtab::get_index("use_compass");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   quan::dynarray<uint8_t> bytestream{sizeof (uint8_t),main_alloc_failed};
   if(!bytestream.good()){
      return false;
   }
   *bytestream.get() = (val == true)?1:0;
   return flash_symtab::write_from_bytestream(sym_index,bytestream);
}

bool set_flash_mag_offset(quan::three_d::vect<float> const & src)
{
   int32_t const sym_index = flash_symtab::get_index("mag_offsets");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   union {
      uint8_t ar[sizeof (float) * 3];
      float vals[3];
   } uconv;
   for ( uint8_t i = 0U; i < 3U; ++i){
      uconv.vals[i] = src[i];
   }
   quan::dynarray<uint8_t> bytestream{sizeof (float) * 3,main_alloc_failed};
   if(!bytestream.good()){
      return false;
   }
   memcpy(bytestream.get(),uconv.ar,sizeof (float) * 3);
   return flash_symtab::write_from_bytestream(sym_index,bytestream);
}

bool get_flash_mag_offset(quan::three_d::vect<float> & dest)
{
   // get string from flash
   int32_t const sym_index = flash_symtab::get_index("mag_offsets");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   uint32_t const length = flash_symtab::get_size(sym_index);
   quan::dynarray<uint8_t> bytestream{length,main_alloc_failed};
   if(!bytestream.good()){
      return false;
   }

   if (!flash_symtab::read(sym_index,bytestream)){
      return false;
   }

   union {
      uint8_t ar[sizeof (float) * 3];
      float vals[3];
   } uconv;

   memcpy(uconv.ar,bytestream.get(),length);
   for (uint8_t i = 0; i < 3U; ++i){
      dest[i] = uconv.vals[i];
   }
   return true;
}

bool get_flash_use_compass( bool & dest)
{
   int32_t const sym_index = flash_symtab::get_index("use_compass");
   if (sym_index == -1){
      // shouldnt get here
      return false;
   }
   uint32_t const length = flash_symtab::get_size(sym_index);
   quan::dynarray<uint8_t> bytestream{length,main_alloc_failed};
   if(!bytestream.good()){
      return false;
   }
   if (!flash_symtab::read(sym_index,bytestream)){
      return false;
   }
   dest = (*bytestream.get() == 1)?true:false;
   return true;
}
