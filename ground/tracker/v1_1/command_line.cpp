
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
   "E"  --> enable azimuth motor
   "D" --> disable azimuth motor
   "$%f,%f,%f;"   --> set home position lat,lon,altitude
   "~%f,%f,%f;"    --> aircraft position  lat,lon,altitude
   "P%i" --> set azimuth position %i
   "Z" --> set zero (North)
   "kP%f" --> set proportional term %f
   "kD%f" --> set differential term %f
   "kC%f"  --> min duty cycle  term %f
   "H%i" ---> set elev servo pos;
   "V%f"  --> set target pan angular velocity

   "GT"  --> get target position
   "GA"  --> get actual position
   "GV"  --> get pan actual angular velocity
   "GZ"  --> get pan emf 0v rail
   "Gz"  --> get pan current 0v rail
   "Gt"  --> get pan target angular velocity
   "Gp"  --> Get proportional term
   "Gd"  --> get differential term  
   "GM" --> get mag reading x,y,z    what this represents is dependendt on the last M+ M- M0 sent prior
   "Gm"  --> get raw mag reading what this represents is dependendt on the last M+ M- M0 sent prior
   "Gf" --> get compass offset // loaded from flash at startup
   "GB" --> get compass bearing. requires a valid mag offset
   "M+" --> Set Mag plus strap gain
   "M-" --> Set Mag - strap gain
   "M0" --> Set Mag - normal reading
   "Mf%f --> set raw mag filter value
// to do 
   "GE" --> get elevation servo pos;
    M!   --> request compass reading disabled (e.g want i2c bus for eeprom access)
    M?   --> is compass reading enabled ( disabling takes a bit to release bus transaction neds to complete)
    M*   --> enable compass reading
    Rd%s 
    Wr%s 
   
*/
namespace {

  // typedef telemetry::gps_position uav_pos_type;

   typedef gcs_serial debug_serial_port;

   typedef quan::uav::position<quan::angle::deg,quan::length::mm> uav_pos_type;

   bool parse_position(const char* cbuf, size_t len, uav_pos_type& pos)
   {
      if ( len > 99){return false;}
      
      char buf[100];
      memcpy(buf,cbuf,len );
      buf[len] ='\0';
      float ar[3] = {0.f,0.f,0.f};
      const char* delims[] = {",",",",";"};

      for ( size_t i = 0; i < 3; ++i){
         char* sptr = (i==0)? buf:nullptr;
         char* f = strtok(sptr,delims[i]);
         if ( f == nullptr ) {return false;}
         quan::detail::converter<float, char*> conv;
         ar[i] = conv(f);
         if(conv.get_errno()){
            return false;
         }
      }
      typedef quan::angle::deg deg;
      typedef quan::length::m m;
      pos = uav_pos_type{deg{ar[0]},deg{ar[1]},m{ar[2]}};
      return true;
   }

   bool parse_home_position(const char* buf,size_t len)
   {
     return false;
     //TODO
     // return parse_position(buf,len,telemetry::m_home_position);
   }

   // buf is a zero terminated string
   void parse_text_command(const char * buf)
   {
       size_t const len = strlen(buf);
       if ( len == 0) {
         return;
       }
       switch (buf[0]){

         case '$' :
               if (! parse_home_position(buf+1,len-1) ){
                  debug_serial_port::write("set home position failed\n");
               }
               else{
                 char buf1[100];
                 sprintf(buf1,"home pos set lat = %.6f, lon = %.6f, alt = %.1f\n",
                  # if 0
                      static_cast<double>(quan::angle::deg{telemetry::m_home_position.lat}.numeric_value()),
                      static_cast<double>(quan::angle::deg{telemetry::m_home_position.lon}.numeric_value()),
                      static_cast<double>(telemetry::m_home_position.alt.numeric_value())
                  #else
                        0.0,0.0,0.0 // TODO
                   #endif
                  );
                 debug_serial_port::write(buf1);
               }
               break;

         case '~' : {
               uav_pos_type aircraft_position;
               if(!parse_position(buf+1,len-1,aircraft_position)){
                  debug_serial_port::write("set aircraft position failed\n");
               }else{
                 char buf1[100];
                 sprintf(buf1,"aircraft pos set lat = %.6f deg, lon = %.6f deg, alt = %.1f mm\n",
                    #if 0
                     static_cast<double>(quan::angle::deg{aircraft_position.lat}.numeric_value()),
                      static_cast<double>(quan::angle::deg{aircraft_position.lon}.numeric_value()),
                      static_cast<double>(aircraft_position.alt.numeric_value())
                     #else
                        0.0,0.0,0.0 // TODO
                     #endif
                  );
                #if 0
                 telemetry::m_aircraft_position = aircraft_position;
                 telemetry::recalc();
                 #endif
                 debug_serial_port::write(buf1);
               }
               break;
         }
         case 'Z' :{
               #if 0
               azimuth::encoder::zero();
               #endif
               debug_serial_port::write("zeroed\n");
         }
         break;
         case 'E' :{
/*
               tracker::pan::enable(true);
               debug_serial_port::write("Azimuth Enabled\n");
*/
            debug_serial_port::write("Function N/A TODO\n");
         }
         break;
         case 'D' :{
/*
               tracker::pan::enable(false);
               debug_serial_port::write("Azimuth Disabled\n");
*/
               debug_serial_port::write("Function N/A TODO\n");
         }
         break;
         case 'P' :
            if ( len > 1){
               #if 0
               uint32_t const pos = atoi(buf+1);
               azimuth::motor::set_target_position(pos);
               char buf1[50];
               sprintf(buf1,"T <~ %ld : OK!\n",azimuth::motor::get_target_position());
               debug_serial_port::write(buf1);
               #else
               debug_serial_port::write("TODO\n");
               #endif
            }
            else{
               debug_serial_port::write("expected uint");
            } 
         break;
         case 'H' :
            if ( len > 1){
               #if 0
               uint32_t pos = quan::constrain(atoi(buf+1),1000,2000);
               main_loop::set_elevation_servo(quan::time_<uint32_t>::us{pos});
               char buf1[50];
               sprintf(buf1,"H <~ %ld : OK!\n",main_loop::get_elevation_servo().numeric_value());
               debug_serial_port::write(buf1);
               #else
                 debug_serial_port::write("TODO\n");
               #endif
            }
            else{
               debug_serial_port::write("expected uint\n");
            } 
         break;
         //############################################
         case 'V':
#if 1
         debug_serial_port::write("Function N/A TODO\n");
#else
            if ( len > 2){
               quan::detail::converter<float,char*> conv;
               float const v = conv(buf + 1);
               if ( conv.get_errno()==0){
                  tracker::pan::set_angular_velocity(tracker::rad_per_s{tracker::rad{v}});
               }else{
                   debug_serial_port::write("float conv error\n");
               }
            }else {
               debug_serial_port::write("expctd kP or kD + float\n");
            } 
#endif
         break;
         case 'k': 
            #if 1
               debug_serial_port::write("Function N/A TODO\n");
            #else
            if ( len > 3){
                 quan::detail::converter<float,char*> conv;
                 float const v = conv(buf + 2);
                 char buf1[50];
                 if (conv.get_errno() ==0){
                     switch (buf[1]){
                        case 'P' : {
                           tracker::pan::set_kP(v);
                           sprintf(buf1,"kP <~ %f : OK!\n",static_cast<double>(v));
                           debug_serial_port::write(buf1);
                        }
                        break;
                        case 'D':{
                           tracker::pan::set_kD(v);
                           sprintf(buf1,"kD  <~ %f : OK!\n",static_cast<double>(v));
                           debug_serial_port::write(buf1);
                        }
                        break;
                        case 'C':{
                           tracker::pan::set_kC(v);
                           sprintf(buf1,"kC  <~ %f : OK!\n",static_cast<double>(v));
                           debug_serial_port::write(buf1);
                        }
                        break;
                        default:
                           debug_serial_port::write("unknown k command\n");
                        break;
                     }
                 }else{ //
                     debug_serial_port::write("float conv error\n");
                 }
               }else{ 
                 debug_serial_port::write("expctd kP kD kD + float\n");
               } 
           #endif
         break;
         case 'G' :
             if ( len > 1){
               switch (buf[1]){
                     case 'T':{
                        #if 0
                        char buf1[50];
                        sprintf(buf1,"Pt = %ld\n",azimuth::motor::get_target_position());
                        debug_serial_port::write(buf1);
                         #else
                            debug_serial_port::write("TODO\n");
                         #endif
                     }
                     break;
                     case 'A':{
                        #if 0
                        char buf1[50];
                        sprintf(buf1,"Pa = %ld\n",azimuth::motor::get_actual_position());
                        debug_serial_port::write(buf1);
                         #else
                           debug_serial_port::write("TODO\n");
                         #endif
                     }
                     break;
                     case 'p':{
                        #if 0
                        char buf1[50];
                        sprintf(buf1,"kP = %f\n",static_cast<double>(azimuth::motor::get_kP()));
                        debug_serial_port::write(buf1);
                          #else
                                 debug_serial_port::write("TODO\n");
                              #endif
                     }
                     break;
                     case 'd': {
                        #if 0
                        char buf1[50];
                        sprintf(buf1,"kD = %f\n",static_cast<double>(azimuth::motor::get_kD()));
                        debug_serial_port::write(buf1);
                            #else
                                 debug_serial_port::write("TODO\n");
                              #endif
                     }
                     break;
                     case 'M':{
                         #if 0
                         quan::three_d::vect<float> const & mag_vect = raw_compass::get_vect();
                         char buf1[100];
                         sprintf(buf1,"mag vector = [%.3f,%.3f,%.3f]\n",
                           static_cast<double>(mag_vect.x),
                              static_cast<double>(mag_vect.y),
                                 static_cast<double>(mag_vect.z)
                         );
                         debug_serial_port::write(buf1);
                         #else
                                 debug_serial_port::write("TODO\n");
                              #endif
                     }
                     break;
                     case 'm':{
                         #if 0
                         quan::three_d::vect<float> const & mag_vect = raw_compass::get_raw();
                         char buf1[100];
                         sprintf(buf1,"raw mag vector = [%.3f,%.3f,%.3f]\n",
                           static_cast<double>(mag_vect.x),
                              static_cast<double>(mag_vect.y),
                                 static_cast<double>(mag_vect.z)
                         );
                         debug_serial_port::write(buf1);
                         #else
                                 debug_serial_port::write("TODO\n");
                              #endif
                     }
                     break;
                     case 'f':{
                           #if 0
                         quan::three_d::vect<float> const & mag_vect = raw_compass::get_offset();
                         char buf1[100];
                         sprintf(buf1,"mag offset = [%.3f,%.3f,%.3f]\n",
                           static_cast<double>(mag_vect.x),
                              static_cast<double>(mag_vect.y),
                                 static_cast<double>(mag_vect.z)
                         );
                         debug_serial_port::write(buf1);
                         #else
                                 debug_serial_port::write("TODO\n");
                              #endif
                     }
                     break;
                     case 'B':{
                           #if 0
                           quan::angle::deg bearing;
                           if (raw_compass::get_bearing(bearing)){
                              char buf1[200];
                              int32_t enc = azimuth::motor::bearing_to_encoder(bearing);
                              quan::angle::deg back = azimuth::motor::encoder_to_bearing(enc);
                              sprintf(buf1,"mag bearing = %.3f deg, encoder value = %ld, back = %.3f\n",
                                          bearing.numeric_value(), enc, back.numeric_value());
                              debug_serial_port::write(buf1);
                           }else{
                              debug_serial_port::write("get_bearing failed\n");
                           }
                            #else
                                 debug_serial_port::write("TODO\n");
                              #endif
                     }
                     break;
               
                     case 'V':{
/*
                          tracker::rad_per_s v = tracker::pan::get_angular_velocity();
                          // n.b implicit conversion from radians value_type
                          double vn = static_cast<double>(v.numeric_value());
                          char buf1[100];
                          sprintf(buf1,"actual angular velocity = %.3f rad.s-1\n",vn);
                          debug_serial_port::write(buf1);
*/
                          debug_serial_port::write("Function N/A TODO\n");
                      }
                      break;
                      case 't': {
/*
                          tracker::rad_per_s v = tracker::pan::get_target_angular_velocity();
                          // n.b implicit conversion from radians value_type
                          double vn = static_cast<double>(v.numeric_value());
                          char buf1[100];
                          sprintf(buf1,"target angular velocity = %.3f rad.s-1\n",vn);
                          debug_serial_port::write(buf1);
*/
                          debug_serial_port::write("Function N/A TODO\n");
                      }
                      break;
                      case  'Z':{
/*
                           quan::voltage::mV v = tracker::pan::get_back_emf_0v_rail();
                           double vn = static_cast<double>(v.numeric_value());
                           char buf1[100];
                           sprintf(buf1,"pan emf 0v rail = %.3f mV\n",vn);
                           debug_serial_port::write(buf1);
*/
                           debug_serial_port::write("Function N/A TODO\n");
                      }
                      break;
                      case  'z':{
/*
                           quan::voltage::mV v = tracker::pan::get_current_0v_rail();
                           double vn = static_cast<double>(v.numeric_value());
                           char buf1[100];
                           sprintf(buf1,"pan current 0v rail = %.3f mV\n",vn);
                           debug_serial_port::write(buf1);
*/
                           debug_serial_port::write("Function N/A TODO\n");
                      }
                      break;
                     default:
                        debug_serial_port::write("unknown get param\n");
                     break;
               }
             }
             else{
               debug_serial_port::write("expctd get param\n");
             }
         break;
         case 'M':{
            #if 0
            if ( len > 1){
               switch(buf[1]){
                  case '0':
                     raw_compass::set_strap(0);
                     debug_serial_port::write("set mag norm\n");
                  break;
                  case '+':
                     raw_compass::set_strap(1);
                     debug_serial_port::write("set mag +strp\n");
                  break;
                  case '-':
                     raw_compass::set_strap(-1);
                     debug_serial_port::write("set mag -strp\n");
                  break;
                  case '!':
                  break;
                  case '?':
                  break;
                  case '*':
                  break;
                  case 'f':{
                     quan::detail::converter<float,char*> conv;
                     float const v = conv(buf + 2);
                     if (conv.get_errno() == 0){
                        if( (v <= 1.0f) && (v > 0.0f) ){
                           raw_compass::set_filter(v);
                           char buf1[60];
                           sprintf(buf1,"raw mag filt set to %f\n",static_cast<double>(v));
                           debug_serial_port::write(buf1);
                        }else{
                           debug_serial_port::write("mag filt float range\n");
                        }
                     }else{
                        debug_serial_port::write("mag filt float invalid\n");
                     }
                  }
                  break;
                  default:
                     debug_serial_port::write("expctd M0, M+, M- mfxxx\n");
                  break;
               }
            }else {
               debug_serial_port::write("expctd M0, M+, M- mfxxx\n");
            }
             #else
                                 debug_serial_port::write("TODO\n");
                              #endif
         }
         break;   
         default:
            debug_serial_port::write("cmd not found\n");
         break;

       }
   }
}

void parse_commandline()
{
   constexpr uint32_t bufsize = 255;
   static char buffer[bufsize];
   static uint32_t index =0;
  // if(debug_serial_port::in_avail()){
      if(index == bufsize){
        // while(debug_serial_port::in_avail()){
           for(;;){
            char ch = debug_serial_port::get();
            if(ch =='\n'){
               break;
            }
           }
         index = 0;
         debug_serial_port::write("command too long\n");
      }else{
         char ch = debug_serial_port::get();
         debug_serial_port::put(ch);
          
         if(ch == '\r'){
            debug_serial_port::write("#\n");
            buffer[index] = '\0';
            index = 0;
            parse_text_command(buffer);
         }else{
            buffer[index++] = ch;
         }
      }
  // }
}