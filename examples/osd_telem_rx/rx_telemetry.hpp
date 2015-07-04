#ifndef QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED

#include <quantracker/osd/telemetry_receiver.hpp>
#include <quan/time.hpp>


// null if not acquired
const char* mutex_acquire_telemetry_string();
// call these only when have mutex 
const char* get_telemetry_string();
quan::time_<int32_t>::ms get_telemetry_received_time();

void mutex_release_telemetry_string();
#endif // QUANTRACKER_AIR_OSD_TELEMETRY_HPP_INCLUDED
