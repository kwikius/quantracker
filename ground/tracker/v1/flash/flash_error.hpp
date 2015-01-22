#ifndef CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED
#define CPP_PROJECTS_TEST_ERROR_HPP_INCLUDED
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
