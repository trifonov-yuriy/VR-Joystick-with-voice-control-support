#include <stm32f30x.h>
#include <stm32f30x_wwdg.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_gpio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <portmacro.h>


#define WWATCH_DOG_TIMER    127

#define WWATCH_DOG_TIMER_LIMIT  10000

void Init_WWatchDog();

