
#include "application.hpp"

#include <FreeRTOS.h>
#include <task.h>

#include <quantracker/osd/osd.hpp>
#include <quantracker/osd/telemetry_transmitter.hpp>
#include "tx_telemetry.hpp"

void create_input_task();
void setup_telemetry_values();

void create_tasks()
{
  create_draw_task();
  create_telemetry_transmitter_task();
  create_input_task();
}

void start_scheduler()
{
  setup_telemetry_values();
  vTaskStartScheduler();
  while (1) {;}
}

