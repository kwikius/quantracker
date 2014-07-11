
#include "tracker_states.hpp"
#include "telemetry.hpp"
#include "azimuth.hpp"
#include "switch_input.hpp"
#include "serial_ports.hpp"
#include "leds.hpp"

namespace {

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
          azimuth::motor::disable();
         // check for user holding down user button (n.b should be true for holding down button!
         if(user_button.get_instant_state() == true){
#ifdef DEBUG
            debug::serial_port::write("quan_tracker V1.1 startup");
#endif
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

     if ( (state==1) && heartbeat_led.is_flashing()){
        error_led.switch_off();
        // ok ready to init tracking...
        startup_led.set_flashing(quan::time_<int32_t>::ms{500},quan::time_<int32_t>::ms{500} );

        pf_on_button_down = on_button_down;
        state = 2;
     }
     // for state 2 start monitoring the data
   }


}//namespace

void on_20ms_event()
{
   //lokking for heartbeats and indicating on heartbeat led
   telemetry::filter_pos();
   if ( telemetry::state_changed){
      // signify by flashing heartbeat
      if( ! heartbeat_led.is_flashing()){
         heartbeat_led.set_flashing(quan::time_<int32_t>::ms{300},quan::time_<int32_t>::ms{700} );
      }
      telemetry::state_changed = false;
   }else{ // no new data for a while...
      if ( heartbeat_led.is_flashing() ){
        static uint32_t telemetry_data_timeout =0;
        if(++telemetry_data_timeout >= 50){
           telemetry_data_timeout = 0;
           heartbeat_led.switch_off();
        }
      }
   }
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
