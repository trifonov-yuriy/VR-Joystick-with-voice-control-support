#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_usart.h>
#include <stm32f30x_rcc.h>
#include "ADC/adc.h"


#define UART4_GPIO                                     GPIOC
#define UART4_GPIO_TX                                  GPIO_Pin_10
#define UART4_GPIO_RX 								   GPIO_Pin_11
#define UART4_GPIO_Pins 							   UART4_GPIO_TX | UART4_GPIO_RX
#define UART4_BAUD_RATE                                115200


void UART4_GPIO_init();
void UART4_init();

void UART_Sender_Task(void *pvParameters);
