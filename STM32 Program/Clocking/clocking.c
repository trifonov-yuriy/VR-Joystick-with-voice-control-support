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
	RCC->CR |= RCC_CR_HSEBYP; //���������� ������������ �� �������� ����������
	RCC_HSEConfig(RCC_HSE_ON);

	RCC_PREDIV1Config(RCC_PREDIV1_Div1);

	RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9); //PLL �� HSE

	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //RCC->CFGR |= RCC_CFGR_SW_PLL;
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5); //48��� �� USB
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //72��� �� AHB
	RCC_PCLK1Config(RCC_HCLK_Div2); //36��� �� APB1
	RCC_PCLK2Config(RCC_HCLK_Div1); //72 ��� �� APB2

	RCC->CR &= ~(1 << 0); //���������� HSI


	RCC_LSICmd(ENABLE); // ��������� ������������ ���������� LSI RC �������

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);// ������� ���������� ������������ ���������� LSI RC �������

	RCC_RTCCLKCmd(ENABLE); // �������� ������������ ������� �������� ����

}
