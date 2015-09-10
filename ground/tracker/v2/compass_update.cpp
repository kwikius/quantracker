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

// TODO add dt e.g time between last read and one before

#include <cstdint>
#include <cstring>
#include <quan/three_d/vect.hpp>
#include <quan/stm32/millis.hpp>
#include <quan/stm32/f4/exti/set_exti.hpp>
#include "resources.hpp"
#include <quan/user.hpp>

namespace {
   const char* do_i2c_errno_str()
   {

      switch(i2c_mag_port::i2c_errno){

         case i2c_mag_port::errno_t::no_error:
            return "no error has been detected";
         case i2c_mag_port::errno_t::invalid_address:
            return "invalid address";
         case i2c_mag_port::errno_t::cant_start_new_transfer_when_i2c_busy:
            return "cant start new transfer when i2c busy";
         case i2c_mag_port::errno_t::zero_data:
            return "zero data";
         case i2c_mag_port::errno_t::data_pointer_is_null:
            return "daya pointer is null";
         case i2c_mag_port::errno_t::invalid_num_bytes_in_rx_multibyte_btf:
            return "invalid numbytes in rx multibyte buffer";
         case i2c_mag_port::errno_t::unexpected_single_total_bytes_in_rx_btf:
            return "unexpected single total bytes in rx btf";
         case i2c_mag_port::errno_t::unexpected_not_last_byte_in_rxne:
            return "unexpetced not last byte in rxne";
         case i2c_mag_port::errno_t::unexpected_flags_in_irq:
            return "unexpected flags in irq";
         case i2c_mag_port::errno_t::i2c_err_handler:
            return "i2c errhandler";
         case i2c_mag_port::errno_t::unknown_exti_irq:
            return "unknown ext irq";
         case i2c_mag_port::errno_t::address_timed_out:
            return "timed out waiting after sending address";
         default:
            return "unlisted i2c error";
      }
   }

   void do_i2c_errno()
   {
     quan::user_message( do_i2c_errno_str());
     quan::user_message("\n");
   }

}

namespace{

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

   bool request_new_measurement()
   {

      if(!i2c_mag_port::transfer_request(mag_write,single_measurement_string,2)){
         quan::user_message("request_new_measurement transfer failed\n");
         do_i2c_errno();
         return false;
      }else{
         vTaskDelay(7);
         return true;
      }
   }

   // to read  lowest address so can auto increment registers
   bool send_x_reg_address()
   {
      if( !i2c_mag_port::transfer_request(mag_write,msb_x_ar,1)){
         quan::user_message("xend x transfer failed\n");
         do_i2c_errno();
         return false;
      }
      return true;
   }

   bool  request_read_mag_values()
   {
       if( !i2c_mag_port::transfer_request(mag_read,local_values,6)){
         quan::user_message("data transfer failed\n");
         do_i2c_errno();
         return false;
      }
      return true;
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
#if 0
   enum class update_state_t {
      new_strap,
      x_address_sent,
      looking_for_exti_event,
      transfer_in_progress,
      transfer_done
   };
   update_state_t update_state = update_state_t::transfer_done;
#endif
   bool request_positive_strap()
   {
      if( !i2c_mag_port::transfer_request(mag_write,positive_pulse_string,2)){
         quan::user_message("positive pulse failed\n");
         do_i2c_errno();
         return false;
      }
      return true;
   }

   bool request_negative_strap()
   {
      if( !i2c_mag_port::transfer_request(mag_write,negative_pulse_string,2)){
         quan::user_message("negative pulse failed\n");
         do_i2c_errno();
         return false;
      }
      return true;
   }

   bool request_no_strap()
   {
      if( !i2c_mag_port::transfer_request( mag_write,no_pulse_string,2)){
         quan::user_message("no pulse failed\n");
         do_i2c_errno();
         return false;
      }
      return true;
   }

}

// needs a redo for rtos
int32_t  ll_update_mag(quan::three_d::vect<int16_t> & result_out,int32_t strap)
{
   if (i2c_mag_port::addr_timeout()){
      return -1;
   }

   if (i2c_mag_port::busy()){
      return 0;
   } 

   if (!request_new_measurement()){
      return -1;
   }

   if (! send_x_reg_address()){
      return -1;
   }
   if( ! request_read_mag_values()){
     return -1;
   }
   copy_new_values(result_out);

   return 1;
}




