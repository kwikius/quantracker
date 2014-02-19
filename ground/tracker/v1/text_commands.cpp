
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
#include "serial_ports.hpp"
#include "azimuth.hpp"
#include "telemetry.hpp"
#include <quan/uav/position.hpp>

/*
   "E"  --> enable azimuth motor
   "D" --> disable azimuth motor
   "$%f,%f,%f;"   --> set home position lat,lon,altitude
   ~%f,%f,%f;"    --> aircraft position  lat,lon,altitude
   "P%i" --> set azimuth position %i
   "Z" --> set zero (North)
   "kP%f" --> set proportional term %f
   "kD%f" --> set differential term %f

   "GT"  --> get target position
   "GA"  --> get actual position
   "Gp"  --> Get proportional term
   "Gd"  --> get differential term  
   "H%i" ---> elev servo pos;

*/
namespace {

   typedef rctx::serial_port cl_sp;
   typedef telemetry::gps_position uav_pos_type;

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
      return parse_position(buf,len,telemetry::m_home_position);
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
                  cl_sp::write("set home position failed\n");
               }
               else{
                 char buf1[200];
                 sprintf(buf1,"home pos set lat= %f , lon= %f, height= %f\n",
                      static_cast<double>(quan::angle::deg{telemetry::m_home_position.lat}.numeric_value()),
                      static_cast<double>(quan::angle::deg{telemetry::m_home_position.lon}.numeric_value()),
                      static_cast<double>(telemetry::m_home_position.alt.numeric_value())
                  );
                 cl_sp::write(buf1);
               }
               break;

         case '~' : {
               uav_pos_type aircraft_position;
               if(!parse_position(buf+1,len-1,aircraft_position)){
                  cl_sp::write("set aircraft position failed\n");
               }else{
                 char buf1[250];
                 sprintf(buf1,"aircraft pos set lat= %f deg, lon= %f deg, height= %f mm\n",
                     static_cast<double>(quan::angle::deg{aircraft_position.lat}.numeric_value()),
                      static_cast<double>(quan::angle::deg{aircraft_position.lon}.numeric_value()),
                      static_cast<double>(aircraft_position.alt.numeric_value())
                  );
                 telemetry::m_aircraft_position = aircraft_position;
                 telemetry::recalc();
                 
                 cl_sp::write(buf1);
               }
               break;
         }
         case 'Z' :{
               azimuth::encoder::zero();
               cl_sp::write("zeroed\n");
         }
         break;
         case 'E' :{
               azimuth::motor::enable();
               cl_sp::write("Azimuth Enabled\n");
         }
         break;

         case 'D' :{
               azimuth::motor::disable();
               cl_sp::write("Azimuth Disabled\n");
         }
         break;
        
         case 'P' :
            if ( len > 1){
               uint32_t const pos = atoi(buf+1);
               azimuth::motor::set_target_position(pos);
               char buf1[50];
               sprintf(buf1,"T <~ %d : OK!\n",azimuth::motor::get_target_position());
               cl_sp::write(buf1);
            }
            else{
               cl_sp::write("expected uint");
            } 
         break;

         case 'H' :
            if ( len > 1){
               uint32_t pos = atoi(buf+1);
               if ( pos < 1000){
                  pos = 1000;
               }else{
                  if ( pos > 2000){
                      pos = 2000;
                  }
               }
               main_loop::set_elevation_servo(quan::time_<uint32_t>::us{pos});
               char buf1[50];
               sprintf(buf1,"H <~ %d : OK!\n",main_loop::get_elevation_servo().numeric_value());
               cl_sp::write(buf1);
            }
            else{
               cl_sp::write("expected uint");
            } 
         break;
         case 'k': 
            if ( len > 3){
               switch (buf[1]){
                  case 'P' : 
                  case 'D' : {
                    quan::detail::converter<float,char*> conv;
                    float const v = conv(buf + 2);
                    if (conv.get_errno() ==0){
                        switch (buf[1]){
                           case 'P' : {
                              azimuth::motor::set_kP(v);
                              char buf1[50];
                              sprintf(buf1,"kP <~ %f : OK!\n",static_cast<double>(v));
                              cl_sp::write(buf1);
                           }
                           break;
                           case 'D':
                              azimuth::motor::set_kD(v);
                              char buf1[50];
                              sprintf(buf1,"kD  <~ %f : OK!\n",static_cast<double>(v));
                              cl_sp::write(buf1);
                           break;
                           default:
                              cl_sp::write("internal error\n");
                           break;
                        }
                    }else{
                        cl_sp::write("float conv error\n");
                    }
                  }
                  break;
                  default :
                     cl_sp::write("unknown command\n");
                  break;
               }
            }
            else{
               cl_sp::write("expected kP or kD + float\n");
            } 
         break;
         case 'G' :
             if ( len > 1){
               switch (buf[1]){
                     case 'T':{
                        char buf1[60];
                        sprintf(buf1,"Pt = %u\n",azimuth::motor::get_target_position());
                        cl_sp::write(buf1);
                     }
                     break;
                     case 'A':{
                        char buf1[50];
                        sprintf(buf1,"Pa = %u\n",azimuth::motor::get_actual_position());
                        cl_sp::write(buf1);
                     }
                     break;
                     case 'p':{
                        char buf1[50];
                        sprintf(buf1,"kP = %f\n",static_cast<double>(azimuth::motor::get_kP()));
                        cl_sp::write(buf1);
                     }
                     break;
                     case 'd': {
                        char buf1[50];
                        sprintf(buf1,"kD = %f\n",static_cast<double>(azimuth::motor::get_kD()));
                        cl_sp::write(buf1);
                     }
                     default:
                      cl_sp::write("unknown get param\n");
                     break;
               }
             }
             else{
               cl_sp::write("expected get param\n");
             }
         break;
         default:
            cl_sp::write("cmd not found\n");
         break;

       }
   }
}

void parse_commandline()
{
   constexpr uint32_t bufsize = 255;
   static char buffer[bufsize];
   static uint32_t index =0;
   if(cl_sp::in_avail()){
      if(index == bufsize){
         while(cl_sp::in_avail()){
            char ch = cl_sp::get();
            if(ch =='\n'){
               break;
            }
         }
         index = 0;
         cl_sp::write("command too long\n");
      }else{
         char ch = cl_sp::get();
         cl_sp::put(ch);
          
         if(ch == '\r'){
            cl_sp::write("#\n");
            buffer[index] = '\0';
            index = 0;
            parse_text_command(buffer);
         }else{
            
            buffer[index++] = ch;
         }
      }
   }
}