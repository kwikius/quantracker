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
#include <quan/three_d/vect.hpp>
#include <quan/dynarray.hpp>
#include <quan/conversion/float_convert.hpp>
#include <quan/conversion/float_to_ascii.hpp>
#include <quan/error.hpp>
#include "flash.hpp"
#include "flash_convert.hpp"
#include "flash_type_tags.hpp"
#include "flash_error.hpp"

namespace{
    const char expected_float[] = "expected [float,float,float]";
}

// expects a string of "[float,float,float]" terminated with 0

bool flash_convert<quan::three_d::vect<float> >::text_to_bytestream(
      quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src_in)
{
   if ((src_in.get_num_elements() < 7) ||  (src_in.get() [0] != '[') ) {
      user_error(expected_float);
      return false;
   }
// copy src_in else will be corrupted
   quan::dynarray<char> src {src_in.get_num_elements() -1,main_alloc_failed};
   if (! src.good()) {
      // has reported
      return false;
   }
   // ignore leading '['
   memcpy (src.get(),src_in.get()+1,src_in.get_num_elements()-1); 
   union {
      uint8_t ar[sizeof (float) * 3];
      float vals[3];
   } uconv;
   
   const char* delims[] = {",",",","]"};

   // user_message("in mag string conv...\n");
   for (size_t i = 0; i < 3; ++i) {
      char* sptr = (i==0) ? reinterpret_cast<char*> (src.get()):nullptr;
      char* f = strtok (sptr,delims[i]);
      if (f == nullptr) {
         user_error(expected_float);
         return false;
      }
      quan::detail::converter<float, char*> fconv;
      uconv.vals[i] = fconv (f);
      if (fconv.get_errno()) {
         user_error("invalid float");
         return false;
      }
   }
   if (!dest.realloc (sizeof (float) * 3)) {
      main_alloc_failed();
      return false;
   }
   memcpy (dest.get(),uconv.ar,sizeof (float) * 3);
   return true;
}

// a sequence of 3 floats
bool flash_convert<quan::three_d::vect<float> >::bytestream_to_text(
      quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src)
/*bool rep_to_cstring_Vect3F (quan::dynarray<char>& dest, 
            quan::dynarray<uint8_t> const & src) */
{
   if (src.get_num_elements() != sizeof (float) *3) {
      quan::error(fn_rep_to_cstring_Vect3F,
            quan::detail::stm32_flash_page_corrupted);
      return false;
   }
   
   union {
      uint8_t ar[sizeof (float) *3];
      float val[3];
   } conv;
   memcpy(conv.ar,src.get(),sizeof (float) *3);

    char buf[100];
#if 0
    char* ptr = buf;
    *ptr++ = '[';
    ptr += quan::float_to_ascii<3>( conv.val[0],ptr);
    *ptr++ = ',';
    ptr += quan::float_to_ascii<3>( conv.val[1],ptr);
    *ptr++ = ',';
    ptr += quan::float_to_ascii<3>( conv.val[2],ptr);
    *ptr++ = ']';
    *ptr++ = '\0';
    int const result = ptr - buf;
#else
//###########results in large code but ok if used already#####
   int const result = sprintf (buf,"[%.3f,%.3f,%.3f]",conv.val[0],conv.val[1],conv.val[2]);
   if ( (result <= 0) || (result >= 100)) {
      quan::error(fn_rep_to_cstring_Vect3F, quan::detail::bad_float_range);
      return false;
   }
//#############################################
#endif   
   if (!dest.realloc (result)) {
      main_alloc_failed();
      return false;
   }
   strcpy (dest.get(), buf);
   return true;
}
