#include "IWatchDog/iwatchdog.h"

void Init_IWatchDog()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //Разблокируем доступ к регистрам
	while(IWDG->SR & (1<<0) == 1);
	while(IWDG->SR & (1<<1) == 1);
	IWDG_SetPrescaler(IWDG_Prescaler_4); //Делитель 256. Частота 40 кГц / 256 = 156,25 Гц
	IWDG_SetReload(IWATCH_DOG_COUNTER); //Значение, от которого начнется счет
	//IWDG_SetWindowValue(0);


	IWDG_Enable(); //Запускаем watch-dog

}


void IWatchDog_Change()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //Разблокируем доступ к регистрам
	while(IWDG->SR & (1<<0) == 1);
	while(IWDG->SR & (1<<1) == 1);
	IWDG_SetPrescaler(IWDG_Prescaler_256); //Делитель 256. Частота 40 кГц / 256 = 156,25 Гц
	IWDG_SetReload(IWATCH_DOG_COUNTER_CHANGED); //Значение, от которого начнется счет
	//IWDG_SetWindowValue(0);


	//IWDG_Enable(); //Запускаем watch-dog

}
