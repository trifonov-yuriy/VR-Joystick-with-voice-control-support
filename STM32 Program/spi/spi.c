#include "spi/spi.h"
#include "dma/dma.h"
#include "interrupt_settings/interupt_settings.h"

extern xSemaphoreHandle SPI_Semaphore;

void SPI_GPIO_INIT()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct_SPI;
	GPIO_InitStruct_SPI.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct_SPI.GPIO_Pin = SPI_GPIO_Pins;
	GPIO_InitStruct_SPI.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_SPI.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStruct_SPI);

	GPIO_PinAFConfig(SPI_GPIO, GPIO_PinSource5, GPIO_AF_5);
	GPIO_PinAFConfig(SPI_GPIO, GPIO_PinSource6, GPIO_AF_5);
	GPIO_PinAFConfig(SPI_GPIO, GPIO_PinSource7, GPIO_AF_5);

	GPIO_InitTypeDef GPIO_InitStruct_SPI_CS;
	GPIO_InitStruct_SPI_CS.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_SPI_CS.GPIO_Pin = SPI_GPIO_CS_Pin;
	GPIO_InitStruct_SPI_CS.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_SPI_CS.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO_CS, &GPIO_InitStruct_SPI_CS);

}

void SPI_INIT()
{

	SPI_GPIO_INIT();
	SPI_NVIC_INIT();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct_SPI;
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //По нарастанию сигнала
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //Задний фронт
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

	SPI_Cmd(SPI1, ENABLE);

	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);

	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);

}

void SPI_NVIC_INIT()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef SPI_NVIC_InitStructure;
	SPI_NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	SPI_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	SPI_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	SPI_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&SPI_NVIC_InitStructure);
}

void SPI1_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(SPI_Semaphore, &xHigherPriorityTaskWoken);
	//if(xHigherPriorityTaskWoken == pdTRUE)
	//{
	//	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//}
	SPI_I2S_ClearFlag(SPI1, SPI_I2S_IT_RXNE);

}

void SPI_writeData(uint8_t address, uint8_t dataToWrite)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_3);
	SPI_sendByte(address);
	SPI_sendByte(dataToWrite);
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
}

uint8_t SPI_sendByte(uint8_t byteToSend)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{}
	SPI_SendData8(SPI1, byteToSend);
	while((SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)) {}
	return (uint8_t)SPI_ReceiveData8(SPI1);
}

void SPI3_GPIO_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct_SPI;
	GPIO_InitStruct_SPI.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct_SPI.GPIO_Pin = SPI3_GPIO_Pins;
	GPIO_InitStruct_SPI.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_SPI.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_GPIO, &GPIO_InitStruct_SPI);

	GPIO_PinAFConfig(SPI3_GPIO, GPIO_PinSource3, GPIO_AF_6);
	GPIO_PinAFConfig(SPI3_GPIO, GPIO_PinSource4, GPIO_AF_6);
	GPIO_PinAFConfig(SPI3_GPIO, GPIO_PinSource5, GPIO_AF_6);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct_SPI_CS;
	GPIO_InitStruct_SPI_CS.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_SPI_CS.GPIO_Pin = SPI3_GPIO_CS_Pin;
	GPIO_InitStruct_SPI_CS.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_SPI_CS.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_GPIO_CS, &GPIO_InitStruct_SPI_CS);

	GPIOA->ODR |= SPI3_GPIO_CS_Pin;

}

void SPI3_init()
{

	SPI3_GPIO_init();
	//SPI3_NVIC_INIT();
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //По нарастанию сигнала
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //Передний фронт
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI3, &SPI_InitStructure);

	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);

	SPI_Cmd(SPI3, ENABLE);

	SPI_RxFIFOThresholdConfig(SPI3, SPI_RxFIFOThreshold_QF);

	//SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, DISABLE);

}

//void SPI3_NVIC_INIT()
//{
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	NVIC_InitTypeDef SPI_NVIC_InitStructure;
//	SPI_NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
//	SPI_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
//	SPI_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	SPI_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&SPI_NVIC_InitStructure);
//}
