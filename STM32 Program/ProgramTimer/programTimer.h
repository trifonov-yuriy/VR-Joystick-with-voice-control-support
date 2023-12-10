#include <stm32f30x_gpio.h>
#include <FreeRTOSConfig.h>
#include <projdefs.h>
#include <portmacro.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <croutine.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

void TimerFunction_one_shot(xTimerHandle RTOS_tim_1);

void StartMeasurement(xTimerHandle RTOS_tim_2);
