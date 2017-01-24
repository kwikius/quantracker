#ifndef QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED

#include <FreeRTOS.h>
#include <task.h>

struct local_interrupt_priority {
     static constexpr uint32_t gcs_serial_port= 15;
     static constexpr uint32_t exti_mag_rdy= 15;
     static constexpr uint32_t i2c_mag_evt = 15;
     static constexpr uint32_t pan_motor = 14;
   //  static constexpr uint32_t video = 11;
};

struct local_task_priority{
   static constexpr uint32_t tracker_task = tskIDLE_PRIORITY + 1UL;
};

#endif // QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED
