#include "resources.hpp"
#include <quan/user.hpp>

namespace quan{
 void user_message(char const * text) 
 {
    typedef sliprings_tx_rx_task sp;
    sp::write(text);
 }
}