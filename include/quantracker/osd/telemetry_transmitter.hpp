#ifndef QUANTRACKER_INCLUDE_OSD_TELEMETRY_TRANSMITTER_HPP_INCLUDED
#define QUANTRACKER_INCLUDE_OSD_TELEMETRY_TRANSMITTER_HPP_INCLUDED

// for size_t
#include <cstdlib>

void setup_telemetry_transmitter_task();
void create_telemetry_transmitter_task();
size_t get_telemetry_transmitter_buffer_length();
size_t write_telemetry_data(const char * buffer, size_t len);

void on_telemetry_transmitted();

#endif // QUANTRACKER_INCLUDE_OSD_TELEMETRY_TRANSMITTER_HPP_INCLUDED
