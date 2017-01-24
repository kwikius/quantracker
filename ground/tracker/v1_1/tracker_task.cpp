
#include "system/resources.hpp"
#include <quan/stm32/flash.hpp>
#include <quan/error.hpp>
#include <quan/user.hpp>
#include "FreeRTOS.h"
#include <task.h>

void parse_commandline();

namespace {

      typedef gcs_serial sp_task;

    
      void tracker_task(void * params)
      {
         for (;;){
            asm volatile ("nop":::);
         }
       }

      char dummy_param  =0;
      TaskHandle_t task_handle = NULL;

     constexpr uint32_t task_stack_size = 3000U;

     StackType_t __attribute__ ((used,section (".task_stacks"))) task_stack[task_stack_size];
     StaticTask_t __attribute__ ((used,section (".task_stacks"))) task_buffer;
}

void create_tracker_task()
{
   task_handle = ::xTaskCreateStatic(
     tracker_task,
     "tracker_task",
     task_stack_size,
     &dummy_param,
     local_task_priority::tracker_task,
     task_stack,
     &task_buffer);

}

namespace {
  StaticTask_t __attribute__ ((used,section (".task_stacks"))) xIdleTaskTCB;
  StackType_t __attribute__ ((used,section (".task_stacks"))) uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
}

extern "C" void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */


    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/
namespace {
   StaticTask_t __attribute__ ((used,section (".task_stacks"))) xTimerTaskTCB;
   StackType_t __attribute__ ((used,section (".task_stacks"))) uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
}
/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
extern "C" void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */


    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}