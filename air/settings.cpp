#include "settings.hpp"

settings::altitude_t settings::altitude_src 
   = settings::altitude_t::baro_alt;

settings::protocol_t settings::frsky_protocol 
   = settings::protocol_t::zapp1;

settings::protocol_t settings::fsk_protocol 
   = settings::protocol_t::zapp2;
