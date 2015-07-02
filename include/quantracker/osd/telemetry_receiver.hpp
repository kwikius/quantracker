#ifndef QUANTRACKER_INCLUDE_OSD_RECEIVE_TELEMETRY_HPP_INCLUDED
#define QUANTRACKER_INCLUDE_OSD_RECEIVE_TELEMETRY_HPP_INCLUDED

// for size_t
#include <cstdlib>
// call to initialise the telemetry recieve subsytem;
 void setup_telemetry_receiver_task();
// call to init the telemetry_receive_task
 void create_telemetry_receiver_task();
// callback when a new buffer has been received
 void on_telemetry_received();
// get size of buffer
 size_t get_telemetry_receiver_buffer_length();
// read the telemetry buffer
 size_t read_telemetry_data(char * buffer, size_t len);

#endif // QUANTRACKER_INCLUDE_OSD_RECEIVE_TELEMETRY_HPP_INCLUDED
