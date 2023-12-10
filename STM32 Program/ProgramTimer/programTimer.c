#include "ProgramTimer/programTimer.h"

extern xSemaphoreHandle        SettingsSet_Semaphore;
extern xSemaphoreHandle        L3GD20_Start_Semaphore;
extern xSemaphoreHandle        LSM303DLHC_Start_Semaphore;

void TimerFunction_one_shot(xTimerHandle RTOS_tim_1)
{

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		xSemaphoreGive(SettingsSet_Semaphore);
	}

	xTimerDelete(RTOS_tim_1, portMAX_DELAY);
}


void StartMeasurement(xTimerHandle RTOS_tim_startMeasurement)
{
	xSemaphoreGive(L3GD20_Start_Semaphore);
	xSemaphoreGive(LSM303DLHC_Start_Semaphore);
}
