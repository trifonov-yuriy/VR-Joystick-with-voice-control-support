#ifndef ADC_H_
#define ADC_H_

#include <stm32f30x_adc.h>
#include <stm32f30x_rcc.h>
#include "TIM1/tim1.h"
//#include "Speex/speexx.h"
//#include "spi/spi.h"
//#include "USART/usart.h"
#include "UART4/uart4.h"
#include "dma/dma.h"

#include "gpio_settings/GPIO.h"

#define ADC_RESULT_SIZE         1000
#define ADC_RESULT_SIZE_COEF    1
#define ESP32_SIZE              30


#define ADC_BUF_SIZE_FOR_UART4  ADC_RESULT_SIZE


//typedef struct
//{
//	uint16_t ResultBuf1[ADC_RESULT_SIZE];
//	uint16_t ResultBuf2[ADC_RESULT_SIZE];
//} ADC_ResultTypeDef;

uint16_t ADC_ResultBuf[ADC_RESULT_SIZE];
char ESP32_ResultBuf[ESP32_SIZE];
uint16_t *ResultToUART4;


//ADC_ResultTypeDef adc_ResultTypeDef;

void ADC_init();

void ADCResult_Task(void *pvParameters);



#endif
