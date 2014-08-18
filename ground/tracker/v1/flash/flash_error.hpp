#ifndef CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED
#define CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED

#include <quan/error.hpp>

void report_errors();
uint32_t get_num_errors();

enum function_ids{
   fn_parse_input = quan::detail::function_id_user+1
   ,fn_main
   ,fn_flash_demo
   ,fn_rep_to_cstring_MagVect3F
   ,fn_rep_to_cstring_Char
   ,fn_rep_to_cstring_CharAr8
   ,fn_cstring_to_rep_magic
   ,fn_cstring_to_rep_Char
   ,fn_cstring_to_rep_String
};

void main_alloc_failed();

void user_error(char const * str);
//nb system errors

#endif // CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED
