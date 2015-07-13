/*
 Copyright (c) 2012 - 2015 Andy Little 

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

#include <iostream>
#include <string>
#include <cstring>
#include <quan/utility/timer.hpp>

#include "flash_vars_upload.hpp"

int flash_write (std::string const & filename,std::string const & port);
int flash_read (std::string const & filename,std::string const & port);

int main (int argc, char const* argv[])
{
   std::cout << "******************************\n";
   std::cout << "*                            *\n";
   std::cout << "*  Quantracker Air OSD       *\n";
   std::cout << "*  Params setup v1.1         *\n";
   std::cout << "*  Copyright (C) Andy Little *\n";
   std::cout << "*  www.zoomworks.org         *\n";
   std::cout << "*  Jul 2015                  *\n";
   std::cout << "*                            *\n";
   std::cout << "******************************\n\n";
   
   if (argc != 5) {
      std::cout << "usage " << argv[0] << " -read|-write <filename> -port <portname>\n";
      return EXIT_FAILURE;
   }

   if (!strncmp (argv[1], "-read", 6)) {
      
      return flash_read(argv[2],argv[4]);
   }

   if ( (!strncmp (argv[1], "-write", 7)) && ( !strncmp (argv[3], "-port",6) ) ){

      return flash_write (argv[2],argv[4]);
   } 
}

void delay (quan::time::s const & t)
{
   quan::timer<> timer;
   auto start_time = timer();
   auto count_time = timer();
   std::cout << "-";
   while ( (timer() - start_time) < t)
   {
      if ( (timer() - count_time) > quan::time::s {0.25}) {
         count_time = timer();
         std::cout << "-";
         std::cout.flush();
      }
   }
   std::cout << "\n";
}

bool get_next_char (quan::serial_port & sp, char & ch_out, quan::time::ms const & wait_time)
{
   quan::timer<quan::time::ms> timer;
   while ( (timer() < wait_time) && !sp.in_avail())
   {;}
      
   if (!sp.in_avail()) {
      return false;
   }
   unsigned char ch;
   sp.read (&ch, 1);
   ch_out = static_cast<char> (ch);
   return true;
}
 
 
 
 
 
 
 