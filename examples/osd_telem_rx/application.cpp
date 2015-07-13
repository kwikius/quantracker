
#include "application.hpp"

#include <FreeRTOS.h>
#include <task.h>

#include <quantracker/osd/osd.hpp>
#include <quantracker/osd/telemetry_receiver.hpp>

void create_tasks()
{
  create_draw_task();
  create_telemetry_receiver_task();
}

void start_scheduler()
{
  vTaskStartScheduler();
  while (1) {;}
}

