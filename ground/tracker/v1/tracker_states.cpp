
#include "tracker_states.hpp"
#include "telemetry.hpp"
#include "azimuth.hpp"
#include "switch_input.hpp"
#include "serial_ports.hpp"
#include "leds.hpp"

namespace {

   bool home_set_flag = false;
   bool error_flag = false;
   bool awaiting_tracker_data = true;

   void initial_20_ms_event();

   void idle(){}
   void (*pf_on_button_down)() = idle;
   void (*pf_on_button_up)() = idle;
   void (*pf_on_20_ms_event)() = initial_20_ms_event;

   void tracking()
   {
     // static uint32_t count = 0;
     // if ( ++ count == 5 ){
       //  count = 0;
//         if( telemetry::state_changed){
//            telemetry::state_changed = false;
            telemetry::recalc();
//            error_led.switch_off();
//         }else{
//            error_led.set_flashing(quan::time_<int32_t>::ms{400},quan::time_<int32_t>::ms{200} );
//         }
//      }
   }

   void init_tracking()
   {
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

   void  on_button_down(){ 
     
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
         pf_on_button_down = initial_on_button_down;
         pf_on_button_up = initial_on_button_up;
         azimuth::motor::disable();
         error_led.set_flashing(quan::time_<int32_t>::ms{400},quan::time_<int32_t>::ms{200} );
         state = 1;
#ifdef DEBUG
         debug::serial_port::write("quan_tracker V1.1 startup");
#endif
     };
     
     if ( (state==1) && telemetry::state_changed ){
        error_led.switch_off();
        startup_led.set_flashing(quan::time_<int32_t>::ms{500},quan::time_<int32_t>::ms{500} );
        heartbeat_led.set_flashing(quan::time_<int32_t>::ms{300},quan::time_<int32_t>::ms{700} );
        pf_on_button_down = on_button_down;
        state = 2;
     }
   }
}//namespace

void on_20ms_event()
{
   telemetry::filter_pos();
   pf_on_20_ms_event();
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
