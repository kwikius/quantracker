
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
#include <quan/utility/fifo.hpp>
#include "flash_error.hpp"

void user_message(const char * str);

namespace {

   const char* get_function_name(int32_t function_id)
   {
      switch (function_id){
         case  quan::detail::stm32_flash_read_symbol:
         return "quan::stm32::flash_read_symbol";
         case  quan::detail::stm32_flash_write_symbol:
         return "quan::stm32::flash_write_symbol";
         case  quan::detail::stm32_flash_get_write_count:
         return "quan::stm32::flash_get_write_count";
         case quan::detail::stm32_ll_flash_get_sym_ptr:
         return "(static) stm32_ll_flash_get_sym_ptr";
// app
         case fn_parse_input:
         return "parse_input";
         case fn_main:
         return "main";
         case fn_rep_to_cstring_Vect3F:
         return "rep_to_cstring_Vect3F";
         case fn_rep_to_cstring_Bool:
         return "rep_to_cstring_Bool";

         default:
         return "unknown function";
         
      }
   }

   const char* get_error_string(int32_t error_id)
   {
      switch (error_id){
         case  quan::detail::stm32_flash_page_corrupted:
         return "flash page corrupted";
         case quan::detail::invalid_storage_size:
         return "invalid storage size";
         case quan::detail::out_of_flash_memory:
         return "out of flash memory";
         case quan::detail::out_of_heap_memory:
         return "out of heap memory";
         case quan::detail::stm32_flash_invalid_symbol_index:
         return "invalid symbol index";
         case quan::detail::stm32_flash_symbol_not_found:
         return "stm32 flash symbol not found";
         case quan::detail::recursion_too_deep:
         return "recursion too deep";
         case quan::detail::unterminated_string_constant:
         return "unterminated string constant";
         default:
         return "unknown error";
      }
   }

   struct error_info{
      int32_t function_id;
      int32_t error_id;
   };

   quan::fifo<error_info,16> error_fifo;
}// ~namespace

void quan::error(int32_t function_id, int32_t error_id)
{
    if (error_fifo.is_full()){
      error_fifo.get();
    }
    error_fifo.put({function_id,error_id});
}

uint32_t get_num_errors()
{
   return error_fifo.num_in_buffer();
}

void report_errors()
{
   while ( ! error_fifo.is_empty()){
      error_info const & info = error_fifo.get();
      user_message("in fn ");
      user_message(get_function_name(info.function_id));
      user_message( " : ");
      user_message(get_error_string(info.error_id));
      user_message("\n");
      
   }
}

void user_error (char const * str) {
   user_message("error : ");
   user_message(str);
   user_message("\n");
}

void main_alloc_failed()
{
   quan::error (fn_main,quan::detail::out_of_heap_memory);
}