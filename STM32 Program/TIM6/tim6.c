#include "TIM6/TIM6.h"
#include "SPI/spi.h"
#include "I2C/i2c.h"
#include "Sensor/sensor.h"
#include "DMA/dma.h"
#include "USART/usart.h"

void TIM6_init()
{
	//TIM6_NVIC_INIT();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_InitStructure;

	TIM_InitStructure.TIM_Prescaler = TIM6_PRESCALLER;
	TIM_InitStructure.TIM_Period = TIM6_PERIOD;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	TIM_TimeBaseInit(TIM6, &TIM_InitStructure);
	//TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
}

void TIM6_NVIC_INIT()
{
	NVIC_InitTypeDef NVIC_InitStructure_TIM6;
	NVIC_InitStructure_TIM6.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure_TIM6.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure_TIM6.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure_TIM6.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure_TIM6);

}

void TIM6_DAC_IRQHandler()
{

	//ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_601Cycles5); //
	//ADC_StartConversion(ADC1);
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

}





