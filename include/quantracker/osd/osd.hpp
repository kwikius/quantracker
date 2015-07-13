#ifndef QUANTRACKER_INCLUDE_OSD_OSD_HPP_INCLUDED
#define QUANTRACKER_INCLUDE_OSD_OSD_HPP_INCLUDED

#include <FreeRTOS.h>
#include <task.h>
#include <quan/uav/osd/api.hpp>

void create_draw_task();
void osd_setup();

#endif // QUANTRACKER_INCLUDE_OSD_H_INCLUDED
