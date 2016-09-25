
#include <cstdio>
#include <stm32f4xx.h>
//#include <misc.h>
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

   void do_flash_vars()
   {
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

   // simulate asynchronous
   // looking for 3 returns
   // while transmitting string
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

} // namespace

void mode_check()
{ 
   //check if user wants to mod flash vars
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
  
   flash_menu_sp::init();
   input_output io;
   bool want_menu = false;
   for ( uint32_t i = 0; i < 60 ; ++i){
      // crude method of timing for a  approx 4 sec delay
      // to allow user to press return 3 x
      // after startup
      // causes a bit of flicker but keeps text static
      if ( io("Press return 3 times for Flash Menu"
             "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b") ){
         want_menu = true;
         break;
      }
   }
   if ( want_menu){
      
      do_flash_vars();
   }else{
      flash_menu_sp::write("\n\nTime is up! ... Exiting to Flight mode\n\n");
      return;
   } 
}
