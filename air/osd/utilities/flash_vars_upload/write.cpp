
#include <quan/serial_port.hpp>
#include <quan/time.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "flash_vars_upload.hpp"

namespace {

   struct param {
      std::string name;
      std::string value;
   };
    
   std::vector<param> params;
   /*
    getline with '\r' or '\n'
   */
   int getline1( std::istream & in,std::string & str)
   {
      // prob set a max length?
     for (;;){
        if(( !in) || in.eof()){
         return str.length();
        }else{
           char ch;
           in.get(ch);
           if (( ch == '\n') || ( ch == '\r')){
             return str.length();
           }else{
             str += ch;
             continue;
           }
        }
     }
   }
}//namespace

int flash_write (std::string const & filename,std::string const & port)
{
   std::ifstream in (filename.c_str());

   if (!in || in.fail()) {
      std::cout << "couldnt open \"" << filename << "\"\n";
      return EXIT_FAILURE;
   }
   std::cout << "reading \"" << filename << "\"\n\n";
   while (in && ! in.eof()) {
      std::string str;
      getline1 (in, str);
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
            auto value = strtok (NULL, "\n\r");
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
   
   quan::serial_port sp (port.c_str());
   sp.init();
   if (! sp.good()) {
      std::cout << "Sorry .. couldnt connect to \"" << port <<  "\"\n";
      return -1;
   }
   else {
      std::cout << "\nConnected to Serial Port ok!\n\n";
   }
   
   if (sp.set_baud (9600) != 0) {
      std::cout << "couldnt set baud\n";
      return -1;
   }

   std::cout << "\n-----------------Ready to Upload-------------------------\n\n";
   std::cout << "Switch on the OSD board and then immediately press any key\n";

   std::cin.get();

   delay (quan::time::s {0.25});

   sp.write ("\r\r\r");

   char dummy_ch;
   while (get_next_char (sp, dummy_ch, quan::time::s {0.25})) {
      std::cout << dummy_ch ;
   }
   
   delay (quan::time::s {0.25});      
   // write new file values to apm
   int num_writes = 0;
   int num_successes = 0;
   int num_fails = 0;
   for (auto p : params) {
      std::cout << "setting " << p.name << " : " << p.value << '\n';
      
      sp.write ("set ");
      sp.write (p.name.c_str(), p.name.length());
      sp.write (" ");
      sp.write (p.value.c_str(), p.value.length());
      sp.write ("\r");

      std::string str;
      while (get_next_char (sp, dummy_ch, quan::time::s {0.1})) {
         str += dummy_ch;
      }
      ++ num_writes;
      if( (str.find("parsing input.....") != std::string::npos) && 
            (str.find("data written to flash") != std::string::npos) ){ 
         std::cout << "OK.. write to flash succeeded!\n";
         ++num_successes;
      }else{
         std::cout << "FAIL .. Write to flash failed\n";
         if ( str.length() != 0){
            std::cout << "OSD returned \"" << str << "\"\n";
         }
         ++num_fails;
      }
      delay (quan::time::s {.2});
   }
   // 
   sp.write("exit\r");
   while (get_next_char (sp, dummy_ch, quan::time::s {0.2})) {
         std::cout << dummy_ch ;
   }
   std::cout << "\nOSD Flash Variables to update  from \"" << filename << "\"\n\n";
   std::cout << "Total flash variables to update    = " << num_writes <<'\n';
   std::cout << "Total number of successful updates = " << num_successes <<'\n';
   std::cout << "Total number of failed updates     = " << num_fails << '\n';
   std::cout << "..............................................\n";
   return EXIT_SUCCESS;
}