
#include <cstdio>
#include <stm32f4xx.h>
#include <misc.h>
#include <FreeRTOS.h>
#include <task.h>
#include <cstring>
#include "resources.hpp"
#include <quan/stm32/flash.hpp>
#include <quan/error.hpp>
#include <quan/user.hpp>
#include <quan/conversion/itoa.hpp>

bool initialise_flash();
bool init_values_from_flash();

void quan::user_message (const char* str)
{
   if ( str){
      for ( uint32_t i = 0; i < strlen(str);++i){
         quan::user_flush_sptx();
         flash_menu_sp::put(str[i]);
      }
   }
}

//not blocking
uint32_t quan::user_in_avail() 
{ 
   return flash_menu_sp::in_avail();
}

char quan::user_get() 
{ 
   while (flash_menu_sp::in_avail() == 0){;}
   char ch = ' ';
   flash_menu_sp::get(ch); 
   return ch;
}

void quan::user_flush_sptx()
{
   while (! flash_menu_sp::tx_reg_empty() ){;}
}

/*
 check to see if the FrSky Tx and Rx pins are jumpered
  or try the 3 rets option
*/

namespace {

   void delay(){
      for ( uint32_t i = 0u; i < 1000; ++i){
         asm volatile("nop":::);
      }
   }

   class input_output{
      int m_user_ret_count;
      public:
      input_output(): m_user_ret_count{0}{}
      bool operator () (const char* str)
      {
         for ( uint32_t i = 0; i < strlen(str);++i){
            while (flash_menu_sp::in_avail() != 0 ){
               char ch = 0;
               if (flash_menu_sp::get(ch) == true){
                  if ( (ch == '\r') || ( ch == '\n')){
                     if ( ++ m_user_ret_count == 3){
                        return true;
                     }  
                  }
               }
            }
            flash_menu_sp::put(str[i]);
         }
         return false;
      }
   };

   void do_flash_vars()
   {
      flash_menu_sp::init();
#if 1
      input_output io;
      bool want_menu = false;
      for ( uint32_t i = 0; i < 30 ; ++i){
        // 99 bytes at 9600 assume transfer takes 1/10th sec
        // then 30 x through gives 3 sec for user
        // (Since this is b4 startup then dont want to use timers etc
        if ( io("***********************************\n"
                "Press return 3 times for Flash Menu\n"
                "***********************************\n") ){
           want_menu = true;
           break;
         }
      }
      if (! want_menu){
         return;
      }
#endif
      flash_menu_sp::write("Quantracker Air OSD 2.1\n");
      flash_menu_sp::write("~~~~Flash menu mode~~~~\n");
      
      quan::stm32::flash::flash_menu();

      quan::report_errors();
      
      flash_menu_sp::write("Exited Flash Menu mode\n");

      init_values_from_flash();

      quan::report_errors();
      quan::user_flush_sptx();
      // turn off sp
   }
}

void mode_check()
{ 

   //check if user wants to mod flash vars
  // also sets up flash on new firmware
  // without which flash cant be modified
  if (! initialise_flash()){
      // set heartbeat_led on permanently symbolise fail
       quan::stm32::module_enable< heartbeat_led_pin::port_type>();
         quan::stm32::apply<
            heartbeat_led_pin
            , quan::stm32::gpio::mode::output
            , quan::stm32::gpio::otype::push_pull
            , quan::stm32::gpio::pupd::none
            , quan::stm32::gpio::ospeed::slow
            , quan::stm32::gpio::ostate::high
         >();
      while (1){;}
  }
  
#if 0
   quan::stm32::module_enable<frsky_rxi_pin::port_type>();
   quan::stm32::apply<
      frsky_rxi_pin
      ,quan::stm32::gpio::mode::input
      ,quan::stm32::gpio::pupd::pull_up
   >();

   // set port sign to non inverted
   quan::stm32::module_enable<frsky_txo_sign_pin::port_type>();
   quan::stm32::apply<
      frsky_txo_sign_pin
      , quan::stm32::gpio::mode::output
      , quan::stm32::gpio::otype::push_pull
      , quan::stm32::gpio::pupd::none
      , quan::stm32::gpio::ospeed::slow
      , quan::stm32::gpio::ostate::low
   >();
 
  // set frsky tx output high
  quan::stm32::module_enable<frsky_txo_pin::port_type>();
  quan::stm32::apply<
      frsky_txo_pin
      , quan::stm32::gpio::mode::output
      , quan::stm32::gpio::otype::push_pull
      , quan::stm32::gpio::pupd::none
      , quan::stm32::gpio::ospeed::slow
      , quan::stm32::gpio::ostate::high
   >();

   delay();

   //If jumpered or open now rxi pin should be high
   if ( quan::stm32::get<frsky_rxi_pin>() == false){
    return;
   }

   quan::stm32::clear<frsky_txo_pin>();

   delay();

   // if jumpered rxi should now be low
   // keep looking since there may be incoming data on the pin
   for ( uint32_t i = 0; i < 1000; ++i){
      asm volatile ("nop":::);
      if ( quan::stm32::get<frsky_rxi_pin>() == true){
        return;
      }
   }
#else

#endif
   do_flash_vars();
}


