#include <quan/error.hpp>
#include "serial_ports.hpp"

void quan::user_message (const char* str)
{
   debug::serial_port::write (str) ;
}

uint32_t quan::user_in_avail() { return debug::serial_port::in_avail();}
char quan::user_get() { return debug::serial_port::get();}

