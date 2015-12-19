#ifndef OSD_ZAPP4_TX_TX_TELEMETRY_HPP_INCLUDED
#define OSD_ZAPP4_TX_TX_TELEMETRY_HPP_INCLUDED

#include "FreeRTOS.h"
#include <task.h>
#include <semphr.h>
#include <quan/uav/position.hpp>
#include <quan/uav/osd/api.hpp>
#include <quantracker/osd/telemetry_transmitter.hpp>
#include <quan/tracker/zapp4/position.hpp>

quan::uav::osd::norm_position_type * mutex_acquire_position(TickType_t ms);
void  mutex_release_position();
void setup_telemetry_values();


#endif // OSD_ZAPP4_TX_TX_TELEMETRY_HPP_INCLUDED
