#include "IWatchDog/iwatchdog.h"

void Init_IWatchDog()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //������������ ������ � ���������
	while(IWDG->SR & (1<<0) == 1);
	while(IWDG->SR & (1<<1) == 1);
	IWDG_SetPrescaler(IWDG_Prescaler_4); //�������� 256. ������� 40 ��� / 256 = 156,25 ��
	IWDG_SetReload(IWATCH_DOG_COUNTER); //��������, �� �������� �������� ����
	//IWDG_SetWindowValue(0);


	IWDG_Enable(); //��������� watch-dog

}


void IWatchDog_Change()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //������������ ������ � ���������
	while(IWDG->SR & (1<<0) == 1);
	while(IWDG->SR & (1<<1) == 1);
	IWDG_SetPrescaler(IWDG_Prescaler_256); //�������� 256. ������� 40 ��� / 256 = 156,25 ��
	IWDG_SetReload(IWATCH_DOG_COUNTER_CHANGED); //��������, �� �������� �������� ����
	//IWDG_SetWindowValue(0);


	//IWDG_Enable(); //��������� watch-dog

}
