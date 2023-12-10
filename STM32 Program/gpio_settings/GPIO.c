#include "gpio_settings/GPIO.h"

extern xSemaphoreHandle		Button_Semaphore;

void GPIO_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct_Leds;
	GPIO_InitStruct_Leds.GPIO_Pin = LEDS;
	GPIO_InitStruct_Leds.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_Leds.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct_Leds.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitStruct_Leds);

	GPIO_InitTypeDef GPIO_InitStruct_Button;
	GPIO_InitStruct_Button.GPIO_Pin = BUTTON;
	GPIO_InitStruct_Button.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct_Button.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct_Button.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStruct_Button);

	GPIO_ext();
}

void GPIO_ADC_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct_Adc;
	GPIO_InitStruct_Adc.GPIO_Pin = ADC_PINS;
	GPIO_InitStruct_Adc.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(ADC_PORT, &GPIO_InitStruct_Adc);
}

void GPIO_ext()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PREEMTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void GPIO_lcd1602_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct_Lcd1602;
	GPIO_InitStruct_Lcd1602.GPIO_Pin = LCD1602_PINS;
	GPIO_InitStruct_Lcd1602.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_Lcd1602.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct_Lcd1602.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOC, &GPIO_InitStruct_Lcd1602);
	GPIOC->ODR |= LCD1602_PINS; //Настройка на выход для предотвращения от короткого замыкания

}

void EXTI0_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(Button_Semaphore, &xHigherPriorityTaskWoken);

	if(xHigherPriorityTaskWoken == pdTRUE)
	{
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}

	EXTI_ClearFlag(EXTI_Line0);
}
