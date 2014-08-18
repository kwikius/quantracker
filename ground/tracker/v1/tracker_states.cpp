/*
 Copyright (c) 2003-2014 Andy Little.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see http://www.gnu.org/licenses./
 */

#include "tracker_states.hpp"
#include "telemetry.hpp"
#include "azimuth.hpp"
#include "switch_input.hpp"
#include "serial_ports.hpp"
#include "leds.hpp"

namespace {

   bool want_interactive_run_mode = false;
}

void setInteractiveRunMode()
{
   want_interactive_run_mode = true;
}

namespace{

   void initial_20_ms_event();

   void idle(){}
   void (*pf_on_button_down)() = idle;
   void (*pf_on_button_up)() = idle;
   void (*pf_on_20_ms_event)() = initial_20_ms_event;

// 20 ms event
   // add look for data. If no new data then stop leds

   void tracking()
   {
      telemetry::recalc();
   }

   void init_tracking()
   {
      // if not using compass then assume user has pointed tracker at North
      azimuth::encoder::zero();
      telemetry::set_home();
      startup_led.switch_on();
      azimuth::motor::enable();
      tracking_led.set_flashing(quan::time_<int32_t>::ms{300},quan::time_<int32_t>::ms{700} );
   }

   void wait1sec()
   {
     static uint32_t count = 0;
     if ( count == 0){
         startup_led.set_flashing(quan::time_<int32_t>::ms{250},quan::time_<int32_t>::ms{250} );
     }
     if(++count == 50){
       init_tracking();
       pf_on_20_ms_event = tracking;
     }
   }

   void on_button_up()
   {
      pf_on_button_down = idle;
      pf_on_button_up = idle;
      pf_on_20_ms_event = wait1sec;
   }

   void  on_button_down()
   { 
      pf_on_button_up = on_button_up;
   }

   void initial_on_button_down()
   {
        error_led.set_flashing(quan::time_<int32_t>::ms{200},quan::time_<int32_t>::ms{100} );
   }

   void initial_on_button_up()
   {
        error_led.set_flashing(quan::time_<int32_t>::ms{400},quan::time_<int32_t>::ms{200} );
   }
  
   void initial_20_ms_event()
   {
     static int32_t state = 0;
     if ( state == 0){
          azimuth::motor::disable();

         if(want_interactive_run_mode == true){

            debug::serial_port::write("entering interactive mode");

            // command line mode
            telemetry::set_protocol(telemetry::protocol_t::command_line);
            // do lights to suit
            error_led.set_flashing(quan::time_<int32_t>::ms{100},quan::time_<int32_t>::ms{900} );
            state = 3;
         }else{ // wait for user to initiate tracking
            //TODO
            // ADD setup compass to point to normal readings
            // init eeprom etc..
            pf_on_button_down = initial_on_button_down;
            pf_on_button_up = initial_on_button_up;
           
            error_led.set_flashing(quan::time_<int32_t>::ms{400},quan::time_<int32_t>::ms{200} );
            state = 1;
         }
     };
     // OK what happens if no data for some time after received data?
     // startup LED is blue

     if ( (state==1) && heartbeat_led.is_on()){
        error_led.switch_off();
        // ok ready to init tracking...
        startup_led.set_flashing(quan::time_<int32_t>::ms{500},quan::time_<int32_t>::ms{500} );
        pf_on_button_down = on_button_down;
        state = 2;
     }
     // for state 2 start monitoring the data
   }

  
void do_heartbeat()
{
   static bool in_heartbeat = false;
   static uint32_t count = 0;
   if ( in_heartbeat){
      ++count;
      if ( count == 5){
         heartbeat_led.switch_off();
      }else {
         if (count == 10){
            in_heartbeat = false;
         }
      }
   }else{ // not in heartbeat
      if ( telemetry::state_changed ){
         heartbeat_led.switch_on();
         telemetry::state_changed = false;
         in_heartbeat = true;
         count = 0;
      }
   }
}

}//namespace

void on_20ms_event()
{
   pf_on_20_ms_event();
   telemetry::filter_pos();
   do_heartbeat();
   update_leds();
}

void on_change_button_state(bool new_state)
{
   if ( new_state){
      pf_on_button_down();
   }else{
      pf_on_button_up();
   }
}
