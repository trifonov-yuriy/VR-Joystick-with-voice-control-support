#include "ADC/adc.h"

//extern xSemaphoreHandle        DMA_ADC_IT_HT_Semaphore;
extern xSemaphoreHandle        DMA_ADC_IT_TC_Semaphore;
extern xQueueHandle            ADC_Result_Queue_DEBUG;
extern xSemaphoreHandle        UART_Start_Semaphore;
//#define DEBUG



void ADC_init()
{
   ResultToUART4 = &ADC_ResultBuf;

   GPIO_ADC_init();
#ifndef DEBUG
   DMA_initChannel1();
   //DMA2_initChannel2();
  // Speex_Init();
#endif
   //DMA_initChannel3();

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
   RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div6); //Делитель 10 72/6 = 12 МГц < 14 (максимальная частота работы АЦП)

   //ADC_Cmd(ADC1, ENABLE); //Калибровка
   vTaskDelay(1);
   ADC_StartCalibration(ADC1);
   while(ADC_GetCalibrationStatus(ADC1) == SET);




   ADC_InitTypeDef ADC_InitStructure;
   ADC_StructInit(&ADC_InitStructure);

   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

   ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_13;
   ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;

   //ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
   //ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;

   ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
   ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;

   ADC_InitStructure.ADC_NbrOfRegChannel = 1;

   ADC_Init(ADC1, &ADC_InitStructure);

   //ADC1->SQR1 |= ADC_SQR1_SQ1 & (2 << 6); //Второй канал будет опрошен первым биты SQ1 регистра SQR = 2



   //ADC_ExternalTriggerConfig(ADC1, ADC_ExternalTrigConvEvent_13, ADC_ExternalTrigEventEdge_RisingEdge);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_601Cycles5); //
//#ifndef DEBUG
    ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
    ADC_DMACmd(ADC1, ENABLE);


    //ADC_ExternalTriggerConfig(ADC1, ADC_ExternalTrigConvEvent_13, ADC_ExternalTrigEventEdge_RisingEdge);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_601Cycles5); //

//#else
	NVIC_InitTypeDef ADC_NVIC_InitStructureChannel;
	ADC_NVIC_InitStructureChannel.NVIC_IRQChannel = ADC1_2_IRQn;
	ADC_NVIC_InitStructureChannel.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
	ADC_NVIC_InitStructureChannel.NVIC_IRQChannelSubPriority = 0;
	ADC_NVIC_InitStructureChannel.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&ADC_NVIC_InitStructureChannel);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

//#endif
    ADC_Cmd(ADC1, ENABLE);
    ADC_StartConversion(ADC1);

}


void ADC1_2_IRQHandler()
{
	//ADC_DMACmd(ADC1, ENABLE);
	//GPIOE->ODR ^= GPIO_Pin_9;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	//xQueueSendFromISR(ADC_Result_Queue_DEBUG, &ADC1->DR, &xHigherPriorityTaskWoken);
	//if(xHigherPriorityTaskWoken == pdTRUE)
	//{
	//	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//}
	xSemaphoreGiveFromISR(UART_Start_Semaphore, &xHigherPriorityTaskWoken);
	if(xHigherPriorityTaskWoken == pdTRUE)
	{
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	//GPIOE->ODR ^= 1 << 11;
	ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}




void ADCResult_Task(void *pvParameters)
{
	ADC_init();
	TIM6_init();
	//SPI3_init();
	UART4_init();
	//DMA2_initChannel5();
	//DMA2_initChannel1();
	uint8_t counterADC = 1;


	//uint16_t *data;
	uint16_t V;
	while(1)
	{
		IWDG_ReloadCounter();
#ifndef DEBUG
		//if(xQueueSemaphoreTake(DMA_ADC_IT_HT_Semaphore, 0) == pdTRUE)
		//{
        //    Start_Encoding = 1;
        //    EncodingVoice();
		//}

		if(xQueueSemaphoreTake(DMA_ADC_IT_TC_Semaphore, 0) == pdTRUE)
		{
			//TIM_Cmd(TIM6, DISABLE);
            //Start_Encoding = 2;
           // EncodingVoice();
			counterADC++;
			if(counterADC > ADC_RESULT_SIZE_COEF)
			{
				GPIOE->ODR |= 1 << 12;
				ADC_Cmd(ADC1, DISABLE);
			}
			SendToUART4();

		}


		//data = &IN_Buffer[0];
#endif
         //vTaskDelay(200);
         //GPIOE->ODR ^= GPIO_Pin_10;
        // ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_601Cycles5); //
         //ADC_StartConversion(ADC1);
         //V = ADC_GetConversionValue(ADC1);
        //data = adc_ResultTypeDef.ResultBuf1;
        //data = adc_ResultTypeDef.ResultBuf2;
        //data = ADC_ResultBuf;
	}
}


