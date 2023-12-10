#include "UART4/uart4.h"

extern xSemaphoreHandle        UART_Start_Semaphore;

void UART4_GPIO_init()
{
	GPIO_InitTypeDef UART4_GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	UART4_GPIO_InitStructure.GPIO_Pin = UART4_GPIO_Pins;
	UART4_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	UART4_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	UART4_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	UART4_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &UART4_GPIO_InitStructure);

	GPIO_PinAFConfig(UART4_GPIO, GPIO_PinSource10, GPIO_AF_5);
	GPIO_PinAFConfig(UART4_GPIO, GPIO_PinSource11, GPIO_AF_5);

}

void UART4_init()
{
	UART4_GPIO_init();

	USART_InitTypeDef UART4_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	USART_StructInit(&UART4_InitStructure);
	UART4_InitStructure.USART_BaudRate = UART4_BAUD_RATE;

	USART_Init(UART4, &UART4_InitStructure);
	//USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

	USART_Cmd(UART4, ENABLE);

}


void UART_Sender_Task(void *pvParameters)
{
	while(1)
	{

		xQueueSemaphoreTake(UART_Start_Semaphore, portMAX_DELAY);

		SendToUART4();
		GPIOE->ODR |= 1 << 11;
	}
}

void SendToUART4()
{
	//DMA_Cmd(DMA2_Channel5, ENABLE);
    static uint16_t i = 0;
    static uint8_t j = 0;
    for(j = 0; j < ADC_RESULT_SIZE_COEF; j++)
    {
    	for(i = 0; i < ADC_RESULT_SIZE; i++)
    	{
    		while(USART_GetFlagStatus(UART4, USART_ISR_TC) == RESET || USART_GetFlagStatus(UART4, USART_ISR_TXE) == RESET);
    		USART_SendData(UART4, ADC_ResultBuf[i] & 0xFF);
    		while(USART_GetFlagStatus(UART4, USART_ISR_TC) == RESET || USART_GetFlagStatus(UART4, USART_ISR_TXE) == RESET);
    		USART_SendData(UART4, ADC_ResultBuf[i] >> 8);
    	}
    }
    GPIOE->ODR |= 1 << 11;
}
