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

#include <quan/serial_port.hpp>
#include <quan/utility/timer.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

/*
   Since OSD port is designed for manual io)
   leave ample time between commands
*/

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
 
struct param {
   std::string name;
   std::string value;
};
 
std::vector<param> params;
 
/*
  osd_setup -write filename -port <filename>

  e.g osd_setup -write  /home/me/osd_cool_mods.txt -port /dev/ttyUSB0

*/

int main (int argc, char const* argv[])
{
   std::cout << "*************************\n";
   std::cout << "*                       *\n";
   std::cout << "*  Quantracker Air OSD  *\n";
   std::cout << "*  Params setup v1      *\n";
   std::cout << "*  (C) Andy Little      *\n";
   std::cout << "*  www.zoomworks.org    *\n";
   std::cout << "*  Mar 2015             *\n";
   std::cout << "*                       *\n";
   std::cout << "*************************\n\n";
   
   if (argc != 5) {
         std::cout << "usage " << argv[0]  << " -read|-write <filename> -port <portname>\n";
         return EXIT_FAILURE;
      }
   if (!strncmp (argv[1], "-read", 6)) {
      std::cout << "read Not supported yet! sorry!\n";
      return EXIT_FAILURE;
   }
   if (!strncmp (argv[1], "-write", 7)) {
   
      std::ifstream in (argv[2]);

      if (!in || in.fail()) {
         std::cout << "couldnt open \"" << argv[2] << "\"\n";
         return EXIT_FAILURE;
      }
      std::cout << "reading \"" << argv[2] << "\"\n";
      while (in && ! in.eof()) {
         std::string str;
         getline (in, str);
         char buf[500];
         strncpy (buf, str.c_str(),500);
         auto name = strtok (buf, " \t:");
         if (name) {
            if ( (name[0] == '#') || (name[0] == '\0')) {
               continue;
            }
            else {
               auto colon = strtok (NULL, " \t");
               if ( (! colon) || (colon[0] != ':')) {
                  std::cout << "expected colon\n";
                  continue;
               }
               auto value = strtok (NULL, " \n\r");
               if (value) {
                  std::cout << name << " value = " << value << '\n';
                  params.push_back ({name, value});
               }
               else {
                  std::cout << "expected value\n";
               }
            }
         }
      } 
      std::cout << "\n-----------------Preparing to Upload-------------------------\n\n";
#if 0   
     // to test without opening port    
      for (auto p : params) {
         std::cout << "set " << (p.name.c_str()) << " " << p.value.c_str() << '\n';
      }  
//############################################################
#else
      quan::serial_port sp (argv[4]);
      sp.init();
      if (! sp.good()) {
         std::cout << "Sorry .. couldnt connect to \"" << argv[4] <<  "\"\n";
         return -1;
      }
      else {
         std::cout << "Connected to Serial Port ok!\n\n";
      }

      if (sp.set_baud (9600) != 0) {
         std::cout << "couldnt set baud\n";
         return -1;
      }

      char dummy_ch;
      while (get_next_char (sp, dummy_ch, quan::time::s {0.25})) {
         std::cout << dummy_ch ;
      }
      delay (quan::time::s {0.5});      
      // write new file values to apm
      for (auto p : params) {
         std::cout << "setting " << p.name << " : " << p.value << '\n';
         
         sp.write ("set ");
         sp.write (p.name.c_str(), p.name.length());
         sp.write (" ");
         sp.write (p.value.c_str(), p.value.length());
         sp.write ("\r");
         std::cout << "OSD returned :\n";
         while (get_next_char (sp, dummy_ch, quan::time::s {0.2})) {
            std::cout << dummy_ch ;
         }

         delay (quan::time::s {.2});
      }
      std::cout << "\nOSD Flash Variables Updated from \"" << argv[2] << "\"\n\n >~~~~ bye :) ~~~~>\n\n";
#endif
   }  
}
 
 
 
 
 
 