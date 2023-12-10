#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_usart.h>
#include <stm32f30x_rcc.h>
#include "LSM303DLHC/lsm303dlhc.h"
#include "L3GD20/l3gd20.h"
#include <portmacro.h>
#include "CalmanFilter/calmanFilter.h"
#include "ADC/adc.h"

#define USART_GPIO		 								GPIOA
#define USART_GPIO_Pins 								GPIO_Pin_9 | GPIO_Pin_10
#define USART_BAUD_RATE                                 19200



void USART_GPIO_init();

void USART_init();

void USARTSend_Task(void *pvParameters);
