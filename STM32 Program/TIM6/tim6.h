#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_spi.h>
#include <stm32f30x_dma.h>
//#include <stdbool.h>
#include "L3GD20/l3gd20.h"

#define TIM6_PRESCALLER  72
#define TIM6_PERIOD      125

void TIM6_NVIC_INIT();
void TIM6_init();
void TIM6_DAC_IRQHandler();
