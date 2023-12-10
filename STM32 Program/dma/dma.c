#include "dma/dma.h"
#include "L3GD20/l3gd20.h"
#include "LSM303DLHC/lsm303dlhc.h"
//#include "Sensor/sensor.h"
//#include "USART/usart.h"

//receiveData[DMA_BUFF_SIZE] =  {0, 0, 0};

extern xSemaphoreHandle	       SPI_DMA_Semaphore;
extern xSemaphoreHandle        I2C_DMA_Semaphore;
extern xSemaphoreHandle        DMA_ADC_IT_HT_Semaphore;
extern xSemaphoreHandle        DMA_ADC_IT_TC_Semaphore;


void DMA_initChannel1()
{
	DMA_NVIC_initChannel1();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef DMA_InitStructureChannel1;

	DMA_InitStructureChannel1.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructureChannel1.DMA_MemoryBaseAddr = (uint32_t)ADC_ResultBuf;
	DMA_InitStructureChannel1.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructureChannel1.DMA_BufferSize = ADC_RESULT_SIZE;
	DMA_InitStructureChannel1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructureChannel1.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructureChannel1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructureChannel1.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructureChannel1.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructureChannel1.DMA_Priority = DMA_Priority_High;
	DMA_InitStructureChannel1.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructureChannel1);
	DMA_Cmd(DMA1_Channel1, ENABLE);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE);
}

void DMA_initChannel2()
{
	DMA_NVIC_initChannel2();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef DMA_InitStructureChannel2;

	DMA_InitStructureChannel2.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructureChannel2.DMA_MemoryBaseAddr = (uint32_t)L3GD20->receiveData;
	DMA_InitStructureChannel2.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructureChannel2.DMA_BufferSize = DMA_BUFF_SIZE;
	DMA_InitStructureChannel2.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructureChannel2.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructureChannel2.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructureChannel2.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructureChannel2.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructureChannel2.DMA_Priority = DMA_Priority_High;
	DMA_InitStructureChannel2.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructureChannel2);
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
}

//void DMA_initChannel3()
//{
//	DMA_NVIC_initChannel3();
//
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//
//	DMA_InitTypeDef DMA_InitStructureChannel3;

//	DMA_InitStructureChannel3.DMA_PeripheralBaseAddr = (uint32_t)adc_ResultTypeDef.ResultBuf1;
//	DMA_InitStructureChannel3.DMA_MemoryBaseAddr = (uint32_t)adc_ResultTypeDef.ResultBuf2;
//	DMA_InitStructureChannel3.DMA_DIR = DMA_DIR_PeripheralSRC;
//	DMA_InitStructureChannel3.DMA_BufferSize = ADC_RESULT_SIZE;
//	DMA_InitStructureChannel3.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
//	DMA_InitStructureChannel3.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructureChannel3.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructureChannel3.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	DMA_InitStructureChannel3.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructureChannel3.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructureChannel3.DMA_M2M = DMA_M2M_Enable;
//	DMA_Init(DMA1_Channel3, &DMA_InitStructureChannel3);
//	DMA_Cmd(DMA1_Channel3, DISABLE);
//	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
//}

/*
void DMA_initChannel4()
{
	DMA_dataCounter = 0;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef DMA_InitStructureChannel4;
	DMA_InitStructureChannel4.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
	DMA_InitStructureChannel4.DMA_MemoryBaseAddr = (uint32_t)&USART_value;
	DMA_InitStructureChannel4.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructureChannel4.DMA_BufferSize = 8;
	DMA_InitStructureChannel4.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructureChannel4.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructureChannel4.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructureChannel4.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructureChannel4.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructureChannel4.DMA_Priority = DMA_Priority_High;
	DMA_InitStructureChannel4.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructureChannel4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
}*/

void DMA_initChannel7()
{
	DMA_NVIC_initChannel7();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef DMA_InitStructureChannel7;
	DMA_InitStructureChannel7.DMA_PeripheralBaseAddr = (uint32_t)&I2C1->RXDR;
	DMA_InitStructureChannel7.DMA_MemoryBaseAddr = (uint32_t)LSM303DLHC->receiveData;
	DMA_InitStructureChannel7.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructureChannel7.DMA_BufferSize = 2 * LSM303DLHC_NUMBER_AXIES;
	DMA_InitStructureChannel7.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructureChannel7.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructureChannel7.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructureChannel7.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructureChannel7.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructureChannel7.DMA_Priority = DMA_Priority_High;
	DMA_InitStructureChannel7.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel7, &DMA_InitStructureChannel7);
	DMA_Cmd(DMA1_Channel7, DISABLE);
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
}

void DMA1_Channel1_IRQHandler()
{
	//L3GD20_resultAnalyse();
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	//if(DMA_GetITStatus(DMA1_IT_HT1))
	//{
	//	xSemaphoreGiveFromISR(DMA_ADC_IT_HT_Semaphore, &xHigherPriorityTaskWoken);
	//	if(xHigherPriorityTaskWoken == pdTRUE)
	//	{
	//		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//	}
	//	Start_Encoding = 1;
//
	//	GPIOE->ODR ^= GPIO_Pin_8;


	//	DMA_ClearITPendingBit(DMA1_IT_HT1);
	//}
	if(DMA_GetITStatus(DMA1_IT_TC1))
	{
		xSemaphoreGiveFromISR(DMA_ADC_IT_TC_Semaphore, &xHigherPriorityTaskWoken);
		if(xHigherPriorityTaskWoken == pdTRUE)
		{

			portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
		}

		//Start_Encoding = 2;
		GPIOE->ODR ^= GPIO_Pin_9;
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}



	//DMA_Cmd(DMA1_Channel1, DISABLE);
	//ADC_DMACmd(ADC1, DISABLE);
	//DMA_Cmd(DMA1_Channel3, ENABLE);

}

void DMA1_Channel2_IRQHandler()
{
	//L3GD20_resultAnalyse();
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(SPI_DMA_Semaphore, &xHigherPriorityTaskWoken);
	//if(xHigherPriorityTaskWoken == pdTRUE)
	//{
	//	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//}
	DMA_ClearITPendingBit(DMA1_IT_TC2);
}

//void DMA1_Channel3_IRQHandler()
//{
//	GPIOE->ODR ^= GPIO_Pin_9;
//	DMA_Cmd(DMA1_Channel3, DISABLE);
//
//	//uint32_t *data;
//	//data = adc_ResultTypeDef.ResultBuf1;
//	//data = adc_ResultTypeDef.ResultBuf2;
//
//	DMA_ClearITPendingBit(DMA1_IT_TC3);
//}

void DMA1_Channel4_IRQHandler()
{
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_ClearITPendingBit(DMA1_IT_TC4);
}

void DMA1_Channel7_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(I2C_DMA_Semaphore, &xHigherPriorityTaskWoken);

	DMA_ClearITPendingBit(DMA1_IT_TC7);
}

void DMA_NVIC_initChannel1()
{
	NVIC_InitTypeDef DMA_NVIC_InitStructureChannel1;
	DMA_NVIC_InitStructureChannel1.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	DMA_NVIC_InitStructureChannel1.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	DMA_NVIC_InitStructureChannel1.NVIC_IRQChannelSubPriority = 0;
	DMA_NVIC_InitStructureChannel1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&DMA_NVIC_InitStructureChannel1);
}

void DMA_NVIC_initChannel2()
{
	NVIC_InitTypeDef DMA_NVIC_InitStructureChannel2;
	DMA_NVIC_InitStructureChannel2.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	DMA_NVIC_InitStructureChannel2.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	DMA_NVIC_InitStructureChannel2.NVIC_IRQChannelSubPriority = 0;
	DMA_NVIC_InitStructureChannel2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&DMA_NVIC_InitStructureChannel2);
}

void DMA_NVIC_initChannel3()
{
	NVIC_InitTypeDef DMA_NVIC_InitStructureChannel3;
	DMA_NVIC_InitStructureChannel3.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	DMA_NVIC_InitStructureChannel3.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	DMA_NVIC_InitStructureChannel3.NVIC_IRQChannelSubPriority = 0;
	DMA_NVIC_InitStructureChannel3.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&DMA_NVIC_InitStructureChannel3);
}

void DMA_NVIC_initChannel4()
{
	NVIC_InitTypeDef DMA_NVIC_InitStructureChannel4;
	DMA_NVIC_InitStructureChannel4.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	DMA_NVIC_InitStructureChannel4.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	DMA_NVIC_InitStructureChannel4.NVIC_IRQChannelSubPriority = 0;
	DMA_NVIC_InitStructureChannel4.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&DMA_NVIC_InitStructureChannel4);
}

void DMA_NVIC_initChannel7()
{
	NVIC_InitTypeDef DMA_NVIC_InitStructureChannel7;
	DMA_NVIC_InitStructureChannel7.NVIC_IRQChannel = DMA1_Channel7_IRQn;
	DMA_NVIC_InitStructureChannel7.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	DMA_NVIC_InitStructureChannel7.NVIC_IRQChannelSubPriority = 0;
	DMA_NVIC_InitStructureChannel7.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&DMA_NVIC_InitStructureChannel7);
}


void DMA2_NVIC_initChannel3()
{
	NVIC_InitTypeDef DMA2_NVIC_InitStructureChannel3;
	DMA2_NVIC_InitStructureChannel3.NVIC_IRQChannel = DMA2_Channel3_IRQn;
	DMA2_NVIC_InitStructureChannel3.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	DMA2_NVIC_InitStructureChannel3.NVIC_IRQChannelSubPriority = 0;
	DMA2_NVIC_InitStructureChannel3.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&DMA2_NVIC_InitStructureChannel3);
}

void DMA2_initChannel3()
{
	DMA2_NVIC_initChannel3();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	DMA_InitTypeDef DMA2_InitStructureChannel3;

	DMA2_InitStructureChannel3.DMA_PeripheralBaseAddr = (uint32_t)&UART4->RDR;
	DMA2_InitStructureChannel3.DMA_MemoryBaseAddr = (uint32_t)ESP32_ResultBuf;
	DMA2_InitStructureChannel3.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA2_InitStructureChannel3.DMA_BufferSize = ESP32_SIZE;
	DMA2_InitStructureChannel3.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA2_InitStructureChannel3.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA2_InitStructureChannel3.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA2_InitStructureChannel3.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA2_InitStructureChannel3.DMA_Mode = DMA_Mode_Circular;
	DMA2_InitStructureChannel3.DMA_Priority = DMA_Priority_High;
	DMA2_InitStructureChannel3.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel3, &DMA2_InitStructureChannel3);
	DMA_Cmd(DMA2_Channel3, DISABLE);

	DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE);
}

void DMA2_NVIC_initChannel5()
{
	NVIC_InitTypeDef DMA2_NVIC_InitStructureChannel5;
	DMA2_NVIC_InitStructureChannel5.NVIC_IRQChannel = DMA2_Channel5_IRQn;
	DMA2_NVIC_InitStructureChannel5.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	DMA2_NVIC_InitStructureChannel5.NVIC_IRQChannelSubPriority = 0;
	DMA2_NVIC_InitStructureChannel5.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&DMA2_NVIC_InitStructureChannel5);
}

void DMA2_initChannel5()
{
	DMA2_NVIC_initChannel5();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	DMA_InitTypeDef DMA2_InitStructureChannel5;

	DMA2_InitStructureChannel5.DMA_PeripheralBaseAddr = (uint32_t)&UART4->TDR;
	DMA2_InitStructureChannel5.DMA_MemoryBaseAddr = (uint32_t)ResultToUART4;
	DMA2_InitStructureChannel5.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA2_InitStructureChannel5.DMA_BufferSize = ADC_BUF_SIZE_FOR_UART4;
	DMA2_InitStructureChannel5.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA2_InitStructureChannel5.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA2_InitStructureChannel5.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA2_InitStructureChannel5.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA2_InitStructureChannel5.DMA_Mode = DMA_Mode_Circular;
	DMA2_InitStructureChannel5.DMA_Priority = DMA_Priority_High;
	DMA2_InitStructureChannel5.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5, &DMA2_InitStructureChannel5);
	DMA_Cmd(DMA2_Channel5, DISABLE);

	DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE);
}

void DMA2_Channel3_IRQHandler()
{
	//L3GD20_resultAnalyse();
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	//xSemaphoreGiveFromISR(SPI_DMA_Semaphore, &xHigherPriorityTaskWoken);
	//if(xHigherPriorityTaskWoken == pdTRUE)
	//{
	//	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//}
	DMA_Cmd(DMA2_Channel3, DISABLE);
	DMA_ClearITPendingBit(DMA2_IT_TC3);
}

void DMA2_Channel5_IRQHandler()
{
	//L3GD20_resultAnalyse();
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	//xSemaphoreGiveFromISR(SPI_DMA_Semaphore, &xHigherPriorityTaskWoken);
	//if(xHigherPriorityTaskWoken == pdTRUE)
	//{
	//	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//}
	DMA_Cmd(DMA2_Channel5, DISABLE);
	//ADC_Cmd(ADC1, ENABLE);
	//GPIOE->ODR |= (1<<11);
	DMA_ClearITPendingBit(DMA2_IT_TC5);
}


