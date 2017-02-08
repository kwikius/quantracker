
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

#include <stm32f4xx.h>
#include <stdlib.h>
#include <stdio.h>

#include <quan/convert.hpp>
#include <quan/conversion/itoa.hpp>
#include <quan/conversion/float_convert.hpp>
#include <quan/constrain.hpp>
#include <quan/uav/position.hpp>
#include "system/tracker_resources.hpp"
#include "azimuth/servo.hpp"
#include "azimuth/motor.hpp"
#include "azimuth/encoder.hpp"
/*
   "A%f" --> In pwm mode set pwm between -1 to 1
   "D"  --> disable azimuth servo
   "Ea"  --> enable azimuth servo in pwm mode
   "Ep"  --> enable azimuth servo in position mode
   "GA" get actual bearing in degrees
   "GT" get target bearing in degrees
   "Gp" get kP
   "Gd" get kD
   "kP%f" set kP to f
   "kD%f" set kD to f
   "kI%f" set KI to f
   "ki%f  set ki to f
   "P%i"--> set position in degrees 0 to 360
   "Z"  --> zero encoder
*/
namespace {

   // buf is a zero terminated string
   void parse_text_command(const char * buf)
   {
       size_t const len = strlen(buf);
       if ( len == 0) {
         return;
       }
       switch (buf[0]){
         case 'A' :{  // set servo pwm raw in proportional pwm mode
            if ( len > 1){
               quan::detail::converter<float,char*> conv;
               float const pwm = conv(buf + 1);
               if ( conv.get_errno()==0){
                  if (azimuth_servo::set_pwm(pwm)){
                     gcs_serial::print<100>("set azimuth pwm to %f\n", static_cast<double>(pwm));
                  }else{
                     gcs_serial::write("set azimuth pwm failed\n");
                  }
               }else{
                  gcs_serial::write("expected A + float\n");
               }
            }
            else{
               gcs_serial::write("expected uint\n");
            } 
            break;
         }
         case 'Z' :{ // zero encoder
               azimuth_encoder::set_index(0U);
               gcs_serial::write("zeroed\n");
         }
         break;
         case 'E' :{
            if ( len > 1){
               switch (buf[1]){  // p, a,v,e
                  case 'a':
                     azimuth_servo::set_mode(azimuth_servo::mode_t::pwm);
                     gcs_serial::write("setting azimuth mode to \"pwm\"\n");
                     if ( azimuth_servo::enable()== true){
                        gcs_serial::write("Azimuth Enabled\n");
                     }else{
                        gcs_serial::write("enable azimuth failed\n");
                     }
                     break;
                  case 'p':
                     azimuth_servo::set_mode(azimuth_servo::mode_t::position);
                     gcs_serial::write("setting azimuth mode to \"position\"\n");
                     if ( azimuth_servo::enable()== true){
                        gcs_serial::write("Azimuth Enabled\n");
                     }else{
                        gcs_serial::write("enable azimuth failed\n");
                     }
                     break;
                  case 'v':
                  case 'e':
                     gcs_serial::write("enabling azimuth in other than pwm TODO\n");
                     break;
                  default: 
                     gcs_serial::write("unknown azimuth mode\n");
                     break;
               }
            }else{  // Represents position mode for backward compatibility
               gcs_serial::write("expected Enable suffix\n");
               break;
            }
         }
         break;
         case 'D' :{
               azimuth_servo::disable();
               gcs_serial::write("Azimuth Disabled\n");
         }
         break;
         case 'P' :
            if ( len > 1){
               quan::angle::deg pos_deg = unsigned_modulo(quan::angle::deg{atoi(buf+1)});
               azimuth_servo::set_target(pos_deg,azimuth_servo::rad_per_s{quan::angle::rad{0}});
               gcs_serial::print<100>("Set position to %f\n",static_cast<double>(pos_deg.numeric_value()));
            }
            else{
               gcs_serial::write("expected uint\n");
            } 
         break;
         case 'k': 
            if ( len > 3){
                 quan::detail::converter<float,char*> conv;
                 float const v = conv(buf + 2);
                 if (conv.get_errno() ==0){
                     switch (buf[1]){
                        case 'P' : 
                           azimuth_servo::set_kP(v);
                           gcs_serial::print<100>("kP <~ %f : OK!\n",static_cast<double>(v));
                           break;
                        case 'D':
                           azimuth_servo::set_kD(v);
                           gcs_serial::print<100>("kD <~ %f : OK!\n",static_cast<double>(v));
                           break;
                        case 'I':
                           azimuth_servo::set_kI(v);
                           gcs_serial::print<100>("kI <~ %f : OK!\n",static_cast<double>(v));
                           break;
                        case 'i':
                           if ( std::abs(v) >= 0.1f){
                              azimuth_servo::set_ki(v);
                              gcs_serial::print<100>("ki <~ %f : OK!\n",static_cast<double>(v));
                           }else{
                              gcs_serial::write("Ki cannot be less than 0.1\n");
                           }
                           break;
                        default:
                           gcs_serial::write("unknown k command\n");
                        break;
                     }
                 }else{ //
                     gcs_serial::write("float conv error\n");
                 }
               }else{ 
                 gcs_serial::write("expected k[PD] + float\n");
               }

         break;
         case 'G' :
             if ( len > 1){
               switch (buf[1]){
                  case 'T':{
                     quan::angle::deg const bearing = azimuth_servo::get_target_bearing();
                     gcs_serial::print<100>("target bearing = %f deg\n",static_cast<double>(bearing.numeric_value()));
                  }
                  break;
                  case 'A':{
                     quan::angle::deg const bearing = azimuth_servo::get_current_bearing();
                     gcs_serial::print<100>("current bearing = %f deg\n",static_cast<double>(bearing.numeric_value()));
                  }
                  break;
                  case 'p':
                     gcs_serial::print<100>("kP = %f\n",static_cast<double>(azimuth_servo::get_kP()));
                  break;
                  case 'd': 
                     gcs_serial::print<100>("kD = %f\n",static_cast<double>(azimuth_servo::get_kD()));
                  break;
                  default:
                     gcs_serial::write("unknown get param\n");
                  break;
                }
             }
             else{
               gcs_serial::write("expected get param\n");
             }
         break; 
         default:
            gcs_serial::write("cmd not found\n");
         break;
       }
   }
}

void parse_commandline()
{
   constexpr uint32_t bufsize = 255;
   static char buffer[bufsize];
   static uint32_t index =0;
  // if(gcs_serial::in_avail()){
      if(index == bufsize){
        // while(gcs_serial::in_avail()){
           for(;;){
            char ch = gcs_serial::get();
            if(ch =='\n'){
               break;
            }
           }
         index = 0;
         gcs_serial::write("command too long\n");
      }else{
         char ch = gcs_serial::get();
         gcs_serial::put(ch);
          
         if(ch == '\r'){
            gcs_serial::write("#\n");
            buffer[index] = '\0';
            index = 0;
            parse_text_command(buffer);
         }else{
            buffer[index++] = ch;
         }
      }
  // }
}