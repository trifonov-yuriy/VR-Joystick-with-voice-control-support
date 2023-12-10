#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_exti.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_syscfg.h>
#include <FreeRTOSConfig.h>
#include <projdefs.h>
#include <portmacro.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <croutine.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include "interrupt_settings/interupt_settings.h"

#define LEDS              GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define BUTTON            GPIO_Pin_0
#define LCD1602_PINS      GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5
#define LCD1602_DATA_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define LCD1602_RS_PIN    GPIO_Pin_4
#define LCD1602_E_PIN     GPIO_Pin_5

#define ADC_PINS           GPIO_Pin_1
#define ADC_PORT          GPIOA

void GPIO_init();

void GPIO_ext();

void GPIO_lcd1602_init();

void GPIO_ADC_init();
