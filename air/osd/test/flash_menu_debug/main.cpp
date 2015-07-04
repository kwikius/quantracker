// find if on LINUX
// stuff
#include <quan/three_d/out/vect.hpp>
#include <quan/stm32/flash.hpp>
#include <quan/stm32/detail/flash.hpp>
#include <quan/stm32/flash/flash_error.hpp>
#include <cstring>
#include <cstdlib>
#include <quan/error.hpp>
#include <quan/user.hpp>

#include <quan/stm32/flash.hpp>
#include <quan/stm32/flash/flash_convert.hpp>

//#include <quan_matters/src/stm32/flash/flash.cpp>
#include <quan/dynarray.hpp>
#include <string>
#include <iostream>
#include <cassert>
#include <cstdio>

#define	NDEBUG

void on_memory_error()
{
   std::cout << "memory _error!!! \n";
}

bool init_flash_pages(uint32_t pagesize);

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

void write_to_file(std::string const & filename);

bool quan::stm32::flash::flash_menu();
int main(int argc, const char* argv[])
{
  // if no args saying pagesize
  // then default
  std::cout << "Flash menu sim\n";
  // defualt pagesize
  uint32_t page_size = 0;
  if ( argc > 1 ){
    if (!strncmp (argv[1], "-pagesize=", 10)) {
       if (strlen(argv[1]) > 10){
         page_size = atoi(argv[1]) +10;
       }else{
            std::cout << "der\n";
       }
    }else{
      std::cout << "unknown option\n";
    }
  }
  if (page_size == 0){
      std::cout << "doing def page\n";
     page_size = 0x4000;
  }
  if (!init_flash_pages(page_size)){
      std::cout << "failed to setup flash pages\n";
  }
  auto const & symtab = quan::stm32::flash::get_app_symbol_table();
  symtab.init();
  quan::stm32::flash::flash_menu();
  quan::report_errors();

  show_page(1);

  std::cout <<"write to file? Y/N";

  char ch;
  std::cin.get(ch);
  if ((ch == 'Y') || ( ch == 'y')){
    write_to_file("flash_vars.cpp");
  }
}



