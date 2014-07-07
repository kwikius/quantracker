#ifndef QUANTRACKER_AIR_DAC_HPP_INCLUDED
#define QUANTRACKER_AIR_DAC_HPP_INCLUDED

#include <cstddef>

namespace fsk {

   bool put(char ch);
   bool write(const char* buf, size_t len);

   void setup(); // defined in fsk_output.cpp
   void setup_event();
   void send_message();

}


#endif //QUANTRACKER_AIR_DAC_HPP_INCLUDED
