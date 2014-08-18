
/*
 Copyright (c) 2013 Andy Little 

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

#include "main_loop.hpp"
#include "azimuth.hpp"
#include "serial_ports.hpp"
#include "leds.hpp"
#include "switch_input.hpp"
#include "compass.hpp"
#include "flash/flash.hpp"
#include <quan/stm32/systick.hpp>

void setup_systick();
void setup_fsk_demod();
bool flash_menu();
void flush_sp_tx();
void setInteractiveRunMode();

// blink error led forever???
extern "C" void __cxa_pure_virtual() { while (1); }
void *__dso_handle;

// till we have op new...
void operator delete (void*p){ ;}

namespace{

   void infinite_loop()
   {
       // TODO need to flash error led or something
      for (;;){;}
   }

   void initial_setup()
   {
      setup_leds();
      setup_switches();
      setup_systick();
      sliprings::serial_port::init(); // for user interactive
      debug::serial_port::write("quan_tracker V1.1 startup\n");
      // check for fails on flash

      if ( ! flash_symtab::init()){
         infinite_loop();
      }
   }
  // if user button is down then command line
  // then user must respond via sp to continue else exit
   enum class user_next_mode {Fail,Normal,InteractiveRun, FlashMenu};
   user_next_mode check_user_button_down()
   {
      quan::time_<int64_t>::ms t = quan::stm32::millis();

      while ( (quan::stm32::millis() - t ) < quan::time_<int64_t>::ms{10}){;}
      
      if(user_button.get_instant_state() == false){
          return user_next_mode::Normal;
      } else{
         for (;;){
            flush_sp_tx();
            debug::serial_port::write("enter\n");
            debug::serial_port::write("F for flash vars\n");
            debug::serial_port::write("I for interactive mode\n");
            debug::serial_port::write("N for Normal startup\n");
            while(!debug::serial_port::in_avail()){;}
            switch(debug::serial_port::get()){
              case 'F':
               return user_next_mode::FlashMenu;
              case 'I':
               return user_next_mode::InteractiveRun;
              case 'N':
               return user_next_mode::Normal;
              default:
               break;
            }
         }
      } 
      // shouldnt get here
      return user_next_mode::Fail;
   }
}

extern "C" void setup()
{
  

   initial_setup();

   user_next_mode ok_to_continue = check_user_button_down();

   switch(ok_to_continue){

      case user_next_mode::Fail:
         infinite_loop();
         break;
      case user_next_mode::FlashMenu:
      /*
        flash_menu only returns fals on bad error
      */
         if ( ! flash_menu()){
            infinite_loop();
         }
         break;
      case user_next_mode::InteractiveRun:
          setInteractiveRunMode();
          break;
      case user_next_mode::Normal:
         // nothing to do
         break;
      default: // shouldnt get here
         infinite_loop();
         break;
   }
   // if 
   main_loop::elevation_servo_setup_ports();
   azimuth::motor::setup_ports();

  // frsky::serial_port::set_irq_priority(interrupt_priority::frsky_serial_port);
   sliprings::serial_port::set_irq_priority(interrupt_priority::sliprings_serial_port);
   av_fsk::serial_port::set_irq_priority(interrupt_priority::av_fsk_serial_port);

  // frsky::serial_port::init();
  // sliprings::serial_port::init();
   av_fsk::serial_port::init();
     // turn off tx output ( was af)
   quan::stm32::apply<
      av_fsk::txo_pin
     , quan::stm32::gpio::mode::input
     , quan::stm32::gpio::pupd::pull_up
   >(); 
   av_fsk::serial_port::set_baudrate<1200,false>();
   setup_fsk_demod();

   azimuth::encoder::setup();
   raw_compass::init();
   main_loop::setup();

}