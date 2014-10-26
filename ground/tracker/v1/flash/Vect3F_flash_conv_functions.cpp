#include <quan/three_d/vect.hpp>
#include <cstring>
#include <cstdio>
#include <quan/conversion/float_convert.hpp>
#include <quan/conversion/float_to_ascii.hpp>
#include <quan/dynarray.hpp>
#include "flash.hpp"
#include "conv_funcs.hpp"

namespace{
    const char expected_float[] = "expected [float,float,float]";
}

// expects a string of "[float,float,float]" terminated with 0
bool cstring_to_rep_Vect3F (quan::dynarray<uint8_t> & dest, quan::dynarray<char> const & src_in)
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
bool rep_to_cstring_Vect3F (quan::dynarray<char>& dest, 
            quan::dynarray<uint8_t> const & src)
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
#if 1
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
      quan::error(fn_rep_to_cstring_Vect3F, error_bad_float_range);
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
