#include <stm32f30x_dma.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_adc.h>
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
#include "ADC/adc.h"


#define DMA_BUFF_SIZE		 9

volatile uint8_t DMA_dataCounter;

void DMA_initChannel1();

void DMA_initChannel2();

void DMA_initChannel4();

void DMA_initChannel7();

void DMA1_Channel2_IRQHandler();

void DMA1_Channel4_IRQHandler();

void DMA1_Channel7_IRQHandler();

void DMA_resultAnalyse();

void DMA2_initChannel3();

void DMA2_initChannel5();
