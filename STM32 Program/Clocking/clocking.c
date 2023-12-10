#include "Clocking/clocking.h"


void SetClockSettings()
{
	uint8_t i = 0;
//	RCC_HSEConfig(RCC_HSE_ON);
//	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == SET)
//	{
//		i++;
//		GPIOE->ODR |= 1 << 10;
//		if(i == 255) return;
//	}

	RCC_DeInit();
	RCC->CR |= RCC_CR_HSEBYP; //Разрешение тактирования от внешнего геренатора
	RCC_HSEConfig(RCC_HSE_ON);

	RCC_PREDIV1Config(RCC_PREDIV1_Div1);

	RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9); //PLL от HSE

	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //RCC->CFGR |= RCC_CFGR_SW_PLL;
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5); //48МГц на USB
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //72МГц на AHB
	RCC_PCLK1Config(RCC_HCLK_Div2); //36МГц на APB1
	RCC_PCLK2Config(RCC_HCLK_Div1); //72 МГц на APB2

	RCC->CR &= ~(1 << 0); //Отключение HSI


	RCC_LSICmd(ENABLE); // Включение тактирования встроенной LSI RC цепочки

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);// Выбрать источником тактирования внутреннюю LSI RC цепочку

	RCC_RTCCLKCmd(ENABLE); // Включить татикрование часовой тактовой шины

}
