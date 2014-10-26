#include <cstring>
#include <cstdio>
#include <quan/dynarray.hpp>
#include "flash.hpp"
#include "conv_funcs.hpp"
 


namespace {
const char expected_bool[] = "expected \"true\" or \"false\"";
}
 
bool cstring_to_rep_Bool(quan::dynarray<uint8_t>& dest, quan::dynarray<char> const & src)
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
 
bool rep_to_cstring_Bool(quan::dynarray<char>& dest, quan::dynarray<uint8_t> const & src)
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
 
 