#ifndef QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED
#define QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED

#include <FreeRTOS.h>
#include <task.h>

struct local_interrupt_priority {
     static constexpr uint32_t sliprings_serial_port= 15;
     static constexpr uint32_t exti_mag_rdy= 15;
     static constexpr uint32_t i2c_mag_evt = 15;
     static constexpr uint32_t pan_motor = 14;
   //  static constexpr uint32_t video = 11;
};

struct local_task_priority{

  // static constexpr uint32_t vsync_telem_rx =( tskIDLE_PRIORITY + 3UL ) ;
  // static constexpr uint32_t draw =( tskIDLE_PRIORITY + 2UL ) ;
   static constexpr uint32_t tracker_mode = tskIDLE_PRIORITY + 1UL;
   static constexpr uint32_t pan_motor = tskIDLE_PRIORITY + 1UL;
   static constexpr uint32_t compass = tskIDLE_PRIORITY + 1UL;
};

#endif // QUANTRACKER_GROUND_TRACKER_V2_COMMON_RESOURCES_HPP_INCLUDED
