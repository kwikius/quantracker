#ifndef CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED
#define CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED

#include <quan/error.hpp>

void report_errors();
uint32_t get_num_errors();

enum function_ids{
   fn_parse_input = quan::detail::function_id_user+1
   ,fn_main
   ,fn_flash_demo
   ,fn_rep_to_cstring_Vect3F
   ,fn_rep_to_cstring_Bool
};

void main_alloc_failed();

void user_error(char const * str);
void user_message (const char* str);
//nb system errors

#endif // CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED
