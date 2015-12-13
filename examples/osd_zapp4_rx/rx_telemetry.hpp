#ifndef QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED
#define QUANTRACKER_AIR_OSD_RX_TELEMETRY_HPP_INCLUDED

#include "FreeRTOS.h"
#include <semphr.h>
#include <quan/uav/position.hpp>
#include <quantracker/osd/telemetry_receiver.hpp>
#include <quan/uav/osd/api.hpp>
#include <quan/time.hpp>

//typedef quan::uav::position<
//   quan::angle_<int32_t>::deg10e7,
//   quan::length_<int32_t>::mm  // 
//> position_type;
// null if not acquired
quan::uav::osd::norm_position_type * mutex_acquire_position(TickType_t ms);
void   mutex_release_position();

#endif // QUANTRACKER_AIR_OSD_TELEMETRY_HPP_INCLUDED
