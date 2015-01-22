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
#include <quan/dynarray.hpp>
#include "flash.hpp"
#include "flash_type_tags.hpp"
#include "flash_convert.hpp"
#include "flash_error.hpp"

namespace {
   const char expected_bool[] = "expected \"true\" or \"false\"";
}
 
bool flash_convert<bool>::text_to_bytestream(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
{
     // input is "true" or "false"
     uint8_t val = 0;
     if (src.get_num_elements() == 5) {
          if ( strcmp(src.get(),"true") == 0 ) {
               val = 1;
          } else {
               user_error(expected_bool);
               return false;
          }
     } else {
          if ( src.get_num_elements() == 6) {
               if ( strcmp(src.get(),"false") != 0 ) {
                    user_error(expected_bool);
                    return false;
               }
          } else {
               user_error(expected_bool);
               return false;
          }
     }
     static_assert(sizeof(bool) == sizeof(uint8_t), "invalid bool size");
     if (!dest.realloc (sizeof(uint8_t))) {
          main_alloc_failed();
          return false;
     }
     *dest.get() = val;
     return true;
}

bool flash_convert<bool>::bytestream_to_text(
      quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src)
//bool rep_to_cstring_Bool(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src)
{
    if (src.get_num_elements() != sizeof (bool)) {
      quan::error(fn_rep_to_cstring_Bool,
      quan::detail::stm32_flash_page_corrupted);
      return false;
   }
   uint8_t val = *src.get();
   if ( val == 0){ // false
       if (!dest.realloc (6)) { 
            main_alloc_failed();
          return false;
        }
        strcpy(dest.get(),"false");
        return true;
   }else{
      if (val == 1){
         if (!dest.realloc (5)) { 
          main_alloc_failed();
          return false;
        }
        strcpy(dest.get(),"true");
        return true;
      }else{
         quan::error(fn_rep_to_cstring_Bool,
         quan::detail::stm32_flash_page_corrupted);
         return false;
      }
   }
}
 
 