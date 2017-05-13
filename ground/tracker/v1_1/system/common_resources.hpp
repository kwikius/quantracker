#ifndef QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED

/*
 Copyright (c) 2017 Andy Little 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include <FreeRTOS.h>
#include <task.h>

struct local_interrupt_priority {
     static constexpr uint32_t gcs_serial_port= 15;
     static constexpr uint32_t modem_serial_port= 15;
     static constexpr uint32_t exti_mag_rdy= 15;
     static constexpr uint32_t i2c_mag_evt = 15;
     static constexpr uint32_t pan_motor = 14;
   //  static constexpr uint32_t video = 11;
};

struct local_task_priority{
   static constexpr uint32_t tracker_task = tskIDLE_PRIORITY + 2UL;
   static constexpr uint32_t button_press_task = tskIDLE_PRIORITY + 3UL;
   static constexpr uint32_t modem_task = tskIDLE_PRIORITY + 3UL;
};

#endif // QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED
