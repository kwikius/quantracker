
#include <cstdint>
#include <cstring>
#include <quan/three_d/vect.hpp>
#include <quan/stm32/systick.hpp>
#include <quan/stm32/f4/exti/set_exti.hpp>
#include "resources.hpp"
#include "serial_ports.hpp"
#include "events.hpp"

namespace{

   typedef sliprings::serial_port serial_port1;
  
   static const uint8_t mag_read = 0x3D;
   static const uint8_t mag_write = 0x3C;
   static const uint8_t configA = 0x0;
   static const uint8_t configB = 0x1;
   static const uint8_t modereg =  0x02;
   static const uint8_t msb_x = 0x03;
   static uint8_t single_measurement_string[] ={ modereg,1};

// could set gain to 8 samples?
   static uint8_t positive_pulse_string[] = {configA,0b00000001};
   static uint8_t negative_pulse_string[] = {configA,0b00000010};
   static uint8_t no_pulse_string[] =       {configA,0b00000000};
   static uint8_t msb_x_ar[] = {msb_x};
   uint8_t local_values[6];

/*
   remenber state must persist while transfer in progress!
*/
   void request_new_measurement()
   {
      if(!i2c_mag_port::transfer_request(mag_write,single_measurement_string,2)){
         serial_port1::write("request_new_measurement transfer failed\n");
      }
   }

   void send_x_reg_address()
   {
      if( !i2c_mag_port::transfer_request(mag_write,msb_x_ar,1)){
         serial_port1::write("xend x transfer failed\n");
      }
   }

   void  request_read_mag_values()
   {
       if( !i2c_mag_port::transfer_request(mag_read,local_values,6)){
         serial_port1::write("data transfer failed\n");
      }
   }

   int16_t convert_to_int16(uint8_t * d)
   {
      union{
         uint8_t in[2] ;
         int16_t out;
      }u;
      u.in[0] = d[1];
      u.in[1] = d[0];
      return u.out;
   }

   void copy_new_values(quan::three_d::vect<int16_t> & result_out)
   {
      result_out.x = convert_to_int16(local_values);
      result_out.y = convert_to_int16(local_values + 4);
      result_out.z = convert_to_int16(local_values + 2);
   }

   enum class update_state_t {
      new_strap,
      x_address_sent,
      looking_for_exti_event,
      transfer_in_progress,
      transfer_done
   };
   update_state_t update_state = update_state_t::transfer_done;

   void request_positive_strap()
   {
      if( !i2c_mag_port::transfer_request(mag_write,positive_pulse_string,2)){
         serial_port1::write("positive pulse failed\n");
      }
   }

   void request_negative_strap()
   {
      if( !i2c_mag_port::transfer_request(mag_write,negative_pulse_string,2)){
         serial_port1::write("negative pulse failed\n");
      }
   }

   void request_no_strap()
   {
      if( !i2c_mag_port::transfer_request(mag_write,no_pulse_string,2)){
         serial_port1::write("no pulse failed\n");
      }
   }

   //static uint8_t configB_setup_string[]= {configB,0b00000001};
}

//1 on new result_out written , 0 if ok but no new vales, -1 on timeout
// if state not update_state_t::transfer_done ( e.g last call returned 1
// then arg is ignored
//##########################################################
// Note very well that the first measurement after changing the strap setting
// will return a measurement using the previous strap settings.
// This isnt stated in the data sheet except it does state similar in relation to
// changing gain.
// Therefore when strap is changed we dump the first one we get and return 0
// just means waiting a bit longer for result
//#############################################################


int32_t  ll_update_mag(quan::three_d::vect<int16_t> & result_out,int32_t strap)
{
   
   if (i2c_mag_port::addr_timeout()){
     // TODO do error message
     // serial_port1::write("i2c addr timeout\n");
      return -1;
   }
   if (i2c_mag_port::busy()){
      return 0;
   } 
   // used for checking exti timeout
   static quan::time_<int64_t>::ms timepoint{0};
   static int32_t oldstrap = -2; // want a strap to start so put out of range here
   static bool different_strap = false;

   switch (update_state){
      case update_state_t::transfer_done:
#if 0
        if (!i2c1_bus_lock.acquire()){
            return 0;
        }
#endif
        if(strap == oldstrap){
            request_new_measurement();
            timepoint = quan::stm32::millis(); // record time to exti
            update_state = update_state_t::looking_for_exti_event;
         }else{   
            different_strap = true;
            oldstrap = strap;
            switch(strap){
               case 0:
                  request_no_strap();
                  break;
               case  1:
                  request_positive_strap();
                  break;
               case -1:
                  request_negative_strap();
                  break;
               default:
                  break;
            }
            update_state = update_state_t::new_strap;
         }
      return 0;
      case update_state_t::new_strap:
         update_state = update_state_t::transfer_done;
      return 0;
      case update_state_t::looking_for_exti_event:
         if(mag_rdy_event.signalled()){
            mag_rdy_event.clear();
            send_x_reg_address();
            update_state = update_state_t::x_address_sent;
         }else{
            /*
               was failing here with 400 kHz rate on long lines > 500 mm say
               But seems ok at 100 kHz. Dunno wht max line length is though..
            */
            if ( ( quan::stm32::millis() - timepoint) > quan::time_<int64_t>::ms{200} ){
              // timepoint = quan::stm32::millis();
             //  serial_port1::write("no exti\n");
                // TODO do error message
#if 0
               i2c1_bus_lock.release();
#endif
               update_state = update_state_t::transfer_done;
               // reset
               return -1;    // failed  to get exti
            }
         }
      return 0;
      case update_state_t::x_address_sent:
         request_read_mag_values();
         update_state = update_state_t::transfer_in_progress;
      return 0;
      case update_state_t::transfer_in_progress:
// transfer done
/* if strap mode has just been changed then dump 
   the first result we got as its for a different mode
   so just dump it rather than confusing matters
   The caller will just have to wait to gp round again
   and do another conversion
*/ 
#if 0
         i2c1_bus_lock.release();
#endif
         update_state = update_state_t::transfer_done;
         
         if (different_strap == true){
            different_strap = false;
            return 0; // no new data
         }else{
            copy_new_values(result_out);
            return 1; // new data
         }
      default:
         return 0;
   }
}



