#include "USART/usart.h"

extern xQueueHandle            CalmanFilter_ResultAngles_Queue_DEBUG;
extern xQueueHandle            ADC_Result_Queue_DEBUG;

void USART_GPIO_init()
{
	GPIO_InitTypeDef USART_GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	USART_GPIO_InitStructure.GPIO_Pin = USART_GPIO_Pins;
	USART_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	USART_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	USART_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	USART_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &USART_GPIO_InitStructure);

	GPIO_PinAFConfig(USART_GPIO, GPIO_PinSource9, GPIO_AF_7);
	GPIO_PinAFConfig(USART_GPIO, GPIO_PinSource10, GPIO_AF_7);

}

void USART_init()
{
	USART_GPIO_init();

	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = USART_BAUD_RATE;
	USART_Init(USART1, &USART_InitStructure);
	//USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	USART_Cmd(USART1, ENABLE);

}

void CalmanFilterAnalyze()
{
	//uint16_t lsm303dlhc_result[LSM303DLHC_NUMBER_AXIES];
	//int16_t l3gd20_result[LSM303DLHC_NUMBER_AXIES];
	//float l3gd20_resultAngles[L3GD20_NUMBER_ANGLES];
	//float lsm303dlhc_resultAngles[LSM303DLHC_NUMBER_RESULT];
	float calmanFilter_resultAngles[CALMAN_FILTER_NUMBER_ANGLES];
	int16_t data;
	uint16_t data1;
	uint16_t data2;
	uint8_t dataUart1;
	uint8_t dataUart2;
	uint8_t dataUart3;
	uint8_t dataUart4;
	uint8_t dataUart5;
	uint8_t dataUart6;
	while(1)
	{

		//xSemaphoreTake(USART_Semaphore, portMAX_DELAY);
		if(xQueueReceive(CalmanFilter_ResultAngles_Queue_DEBUG, calmanFilter_resultAngles, 0) == pdTRUE)
		//if(xQueueReceive(L3GD20_ResultAnalyze_Queue, l3gd20_result, 0) == pdTRUE)
		//vTaskDelay(10);
		//if(xQueueReceive(L3GD20_ResultAngles_Queue_DEBUG, l3gd20_resultAngles, 0) == pdTRUE)
		{
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			GPIOE->ODR ^= GPIO_Pin_15;
			//USART_GetFlagStatus(USART1, USART_CR1_TE)
			//data = l3gd20_resultAngles[L3GD20_xNumberAxie];
			data = calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie];
			data1 = data >= 0 ? data : -data;
			//uint16_t data = abs(l3gd20_result[L3GD20_xNumberAxie]);
			dataUart1 = (data1 << 8) >> 8;
			dataUart2 = data1 >> 8;
			dataUart3 = data < 0 ? 1 : 0;

			//data = l3gd20_resultAngles[L3GD20_yNumberAxie];
			data = calmanFilter_resultAngles[CALMAN_FILTER_yNumberAxie];
			data2 = data >= 0 ? data : -data;
			dataUart4 = (data2 << 8) >> 8;
			dataUart5 = data2 >> 8;
			dataUart6 = data < 0 ? 1 : 0;

			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart1);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart2);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart3);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart4);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart5);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart6);
		}
	}
}

void USARTSend_Task(void *pvParameters)
{
	//CalmanFilterAnalyze();
	//uint16_t lsm303dlhc_result[LSM303DLHC_NUMBER_AXIES];
	//int16_t l3gd20_result[LSM303DLHC_NUMBER_AXIES];
	//float l3gd20_resultAngles[L3GD20_NUMBER_ANGLES];
	//float lsm303dlhc_resultAngles[LSM303DLHC_NUMBER_RESULT];
	uint16_t adcResultBuf = 0;

	uint8_t dataUart1;
	uint8_t dataUart2;
	while(1)
	{

		if(xQueueReceive(ADC_Result_Queue_DEBUG, &adcResultBuf, 0) == pdTRUE)
		{
			GPIOE->ODR ^= GPIO_Pin_15;
			//USART_GetFlagStatus(USART1, USART_CR1_TE)
			//data = l3gd20_resultAngles[L3GD20_xNumberAxie];


			//uint16_t data = abs(l3gd20_result[L3GD20_xNumberAxie]);
			dataUart1 = (adcResultBuf << 8) >> 8;
			dataUart2 = adcResultBuf >> 8;

			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart1);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, dataUart2);

		}
	}
}
