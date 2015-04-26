// find if on LINUX
// stuff
#include <quan/three_d/out/vect.hpp>
#include <quan/stm32/flash.hpp>
#include <quan/stm32/detail/flash.hpp>
#include <quan/stm32/flash/flash_error.hpp>
#include <cstring>
#include <quan/error.hpp>
#include <quan/user.hpp>

#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_convert.hpp>

//#include <quan_matters/src/stm32/flash/flash.cpp>
#include <quan/dynarray.hpp>

#include <iostream>
#include <cassert>
#include <cstdio>

#define	NDEBUG

void on_memory_error()
{
   std::cout << "memory _error!!! \n";
}

void show_page(int n)
{
   uint8_t * p = (uint8_t*) quan::stm32::flash::detail::get_page_address(n);
   if ( p){
     // printf(":");
      for (uint32_t i = 0; i < quan::stm32::flash::detail::get_page_size(n); ++i){

         printf(" %3.3i :",p[i]);
         if ( ((i+1) %8) == 0 ){
            printf ("\n");
         }
      }
      printf("\n");
   }
}

 bool quan::stm32::flash::flash_menu();
int main()
{
  std::cout << "Flash menu sim\n";
  auto const & symtab = quan::stm32::flash::get_app_symbol_table();
  symtab.init();
  quan::stm32::flash::flash_menu();
  quan::report_errors();

}



