#ifndef QUANTRACKER_AIR_OSD_UTLITILES_FLASH_VARS_UPLOAD_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_UTLITILES_FLASH_VARS_UPLOAD_HPP_INCLUDED

#include <quan/serial_port.hpp>
void delay (quan::time::s const & t);
bool get_next_char (quan::serial_port & sp, char & ch_out, quan::time::ms const & wait_time);



#endif // QUANTRACKER_AIR_OSD_UTLITILES_FLASH_VARS_UPLOAD_HPP_INCLUDED
