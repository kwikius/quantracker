#ifndef QUANTRACKER_AIR_DAC_HPP_INCLUDED
#define QUANTRACKER_AIR_DAC_HPP_INCLUDED

#include <cstddef>

namespace fsk {

   bool put(char ch);
   bool write(const char* buf, size_t len);

   void setup();
   void setup_event();
   void send_message();

}

//namespace zapp2{
//   void fsk_send_message();
//}
#endif //QUANTRACKER_AIR_DAC_HPP_INCLUDED
