
#include <string>
#include <iostream>
#include <fstream>
#include <quan/serial_port.hpp>
#include <quan/time.hpp>

#include "flash_vars_upload.hpp"

int flash_read (std::string const & filename,std::string const & port)
{
   quan::serial_port sp (port.c_str());
   sp.init();
   if (! sp.good()) {
      std::cout << "Sorry .. couldnt connect to \"" << port <<  "\"\n";
      return -1;
   }else {
      std::cout << "\nConnected to Serial Port ok!\n\n";
   }
   
   if (sp.set_baud (9600) != 0) {
      std::cout << "couldnt set baud\n";
      return -1;
   }

   std::ofstream out(filename);

   if (!out || out.fail()) {
      std::cout << "couldnt open \"" << filename << "\"\n";
      return EXIT_FAILURE;
   }

   std::cout << "\n-----------------Ready to Download -------------------------\n\n";
   std::cout << "Switch on the OSD board and then immediately press any key\n";

   std::cin.get();

   delay (quan::time::s {0.25});
      
   sp.write("\r\r\r");

   delay (quan::time::s {0.25});

   char ch;
   while (get_next_char (sp, ch, quan::time::s{0.25})) {
     ;
   }

   sp.write("show\r");
   std::string str;
   int line_count = 0;
   ch ='\0';
   while (get_next_char (sp, ch, quan::time::s{0.25})) {
      if ( (ch == '\n') || (ch == '\r')){
         if ( str.length() > 0){
            break;
         }
      }else{
         str += ch;
         ch = '\0';
      }
   }
   if (str.find("----flash symbols-----") == std::string::npos ){
      std::cout << "#" << str <<  "#" << '\n';
      std::cout << "unexpected input.. quitting\n";
      return EXIT_FAILURE;
   }
   str = "";
   line_count = 0;
   ch = '\0';
   while (get_next_char (sp, ch, quan::time::s {0.25})) {
      if ( (ch == '\n') || (ch == '\r')){
         if ( str.find("------------------------") != std::string::npos){
            out << '\n';
            break;
         }else{
           out << str << '\n';
           ++line_count;
           str = "";
         }
      }else{
         str += ch;
      }
   }

   if (line_count == 0){
      std::cout << "no flash variables found\n";
      return EXIT_FAILURE;
   }else{
      std::cout  << line_count << " flash variables  written to \"" << filename << "\"\n";
      return EXIT_SUCCESS;
   }
}