#include "CalmanFilter/calmanFilter.h"

extern xQueueHandle            CalmanFilter_ResultAngles_Queue;
extern xQueueHandle            CalmanFilter_ResultAnglesToL3GD20_Queue;
extern xQueueHandle    	       LSM303DLHC_Result_Queue;
extern xQueueHandle    	       L3GD20_Result_Queue;
extern xTimerHandle RTOS_tim_startMeasurement;

static uint8_t l3gd20_Result_isDone = 0;
static uint8_t lsm303dlhc_Result_isDone = 0;

//void CalmanFilterTask(void *pvParameters)
//{
//
//	L3GD20_Result_TypeDef l3gd20_result; //Результат работы с гироскопом
//	LSM303DLHC_Result_TypeDef lsm303dlhc_result; //Результат работы с акселлерометром
//	CalmanFilter_Result_TypeDef calmanFilter_result; //углы от фильтра
//  float K = 0; //Коэффициент фильтрации
//	float delta = 0; //Разность между показаниями
//	uint8_t i = 0;
//	while(1)
//	{
//		if(xQueueReceive(L3GD20_Result_Queue, &l3gd20_result, 0) == pdTRUE &&
//		   xQueueReceive(LSM303DLHC_Result_Queue, &lsm303dlhc_result, 0) == pdTRUE)
//		{
//			for(i = 0; i < CALMAN_FILTER_NUMBER_ANGLES; i++)
//			{
//				delta = lsm303dlhc_result.angle[i] - l3gd20_result.angle[i];
//				if(delta < 0)
//				{
//					delta = -delta;
//				}
//				if(delta <= DELTA_MIN)
//				{
//					K = K_MIN;
//				}
//
//				else if(delta > DELTA_MIN && delta < DELTA_MAX)
//				{
//					K = (K_MAX - K_MIN) / (DELTA_MAX - DELTA_MIN) * (delta - DELTA_MIN) + K_MIN;
//				}
//
//				else if(delta > DELTA_MAX)
//				{
//					K = K_MAX;
//				}
//				//K = 1;
//				calmanFilter_result.angle[i] = l3gd20_result.angle[i] + K * (lsm303dlhc_result.angle[i] - l3gd20_result.angle[i]);
//			}
//			//K = 1;
//			//xQueueSend(CalmanFilter_ResultAngles_Queue, calmanFilter_result.angle, 0);
//			xQueueSend(CalmanFilter_ResultAnglesToL3GD20_Queue, calmanFilter_result.angle, 0);
//		}
//	}
//}

void CalmanFilter_Check(L3GD20_Result_TypeDef *l3gd20_Result_TypeDef, LSM303DLHC_Result_TypeDef *lsm303dlhc_Result_TypeDef, CalmanFilter_Result_TypeDef *calmanFilter_Result)
{
	if(l3gd20_Result_isDone == 0)
	{
		if(xQueueReceive(L3GD20_Result_Queue, l3gd20_Result_TypeDef, 0) == pdTRUE)
		{
			calmanFilter_Result->counterL3GD20_Wait = 0;
			l3gd20_Result_isDone = 1;
		}
	}
	if(lsm303dlhc_Result_isDone == 0)
	{
	    if(xQueueReceive(LSM303DLHC_Result_Queue, lsm303dlhc_Result_TypeDef, 0) == pdTRUE)
	   {
		   calmanFilter_Result->counterLSM303DLHC_Wait = 0;
		   lsm303dlhc_Result_isDone=1;
	   }
	}

	 if(l3gd20_Result_isDone == 0)
	 {
		 calmanFilter_Result->counterL3GD20_Wait++;
	 }

	 if(lsm303dlhc_Result_isDone == 0)
	 {
		 calmanFilter_Result->counterLSM303DLHC_Wait++;
	 }

	 if((l3gd20_Result_TypeDef->omega[L3GD20_xNumberAxie] != 0 && l3gd20_Result_TypeDef->omega[L3GD20_xNumberAxie] == calmanFilter_Result->omega_L3GD20_prev[L3GD20_xNumberAxie]) ||
			 (l3gd20_Result_TypeDef->omega[L3GD20_yNumberAxie] != 0 && l3gd20_Result_TypeDef->omega[L3GD20_yNumberAxie] == calmanFilter_Result->omega_L3GD20_prev[L3GD20_yNumberAxie]) ||
			 (l3gd20_Result_TypeDef->omega[L3GD20_zNumberAxie] != 0 && l3gd20_Result_TypeDef->omega[L3GD20_zNumberAxie] == calmanFilter_Result->omega_L3GD20_prev[L3GD20_zNumberAxie]))
	 {
		 calmanFilter_Result->counterL3GD20_Freeze++;
	 }
	 else
	 {
		 calmanFilter_Result->counterL3GD20_Freeze = 0;
	 }

	 calmanFilter_Result->omega_L3GD20_prev[L3GD20_xNumberAxie] = l3gd20_Result_TypeDef->omega[L3GD20_xNumberAxie];
	 calmanFilter_Result->omega_L3GD20_prev[L3GD20_yNumberAxie] = l3gd20_Result_TypeDef->omega[L3GD20_yNumberAxie];
	 calmanFilter_Result->omega_L3GD20_prev[L3GD20_zNumberAxie] = l3gd20_Result_TypeDef->omega[L3GD20_zNumberAxie];

	 if(calmanFilter_Result->counterL3GD20_Wait > COUNTER_LIMIT)
	 {
		 calmanFilter_Result->isBreakL3GD20 = 1;
	 }

	 if(calmanFilter_Result->counterLSM303DLHC_Wait > COUNTER_LIMIT)
	 {
		 calmanFilter_Result->isBreakLSM303DLHC = 1;
	 }

	 if(calmanFilter_Result->counterL3GD20_Freeze > COUNTER_LIMIT)
	 {
		 calmanFilter_Result->isBreakL3GD20 = 1;
	 }

	 if(calmanFilter_Result->isBreakL3GD20)
	 {
		 taskENTER_CRITICAL(); //Критическая секция. Вход
		//между макросами taskENTER_CRITICAL() и taskEXIT_CRITICAL()
		//невозможно переключение на другую задачу
		while(1) {}; //Заблокировали всю систему.
		taskEXIT_CRITICAL();  //Критическая секция. Выход
	//Особенностью критических секций является то, что код внутри критической секции не может
	//быть остановлен даже возникающими прерываниями
	 }

	 if(calmanFilter_Result->isBreakL3GD20 && calmanFilter_Result->isBreakLSM303DLHC)
	 {
	 	taskENTER_CRITICAL(); //Критическая секция. Вход
		//между макросами taskENTER_CRITICAL() и taskEXIT_CRITICAL()
		//невозможно переключение на другую задачу
		while(1) {}; //Заблокировали всю систему.
		taskEXIT_CRITICAL();  //Критическая секция. Выход
	//Особенностью критических секций является то, что код внутри критической секции не может
	//быть остановлен даже возникающими прерываниями
	 }
}


void CalmanFilter_Result(L3GD20_Result_TypeDef *l3gd20_Result_TypeDef, LSM303DLHC_Result_TypeDef *lsm303dlhc_Result_TypeDef, CalmanFilter_Result_TypeDef *calmanFilter_result)
{
	 CalmanFilter_Check(l3gd20_Result_TypeDef, lsm303dlhc_Result_TypeDef, calmanFilter_result);
	//xQueueReceive(L3GD20_Result_Queue, l3gd20_Result_TypeDef, portMAX_DELAY);
	//xQueueReceive(LSM303DLHC_Result_Queue, lsm303dlhc_Result_TypeDef, portMAX_DELAY);
	//GPIOE->ODR |= 1<<12;
	 if(l3gd20_Result_isDone && lsm303dlhc_Result_isDone)
	 {

		 float K = 0; //Коэффициент фильтрации
		 float delta = 0; //Разность между показаниями
		 uint8_t i = 0;


		 for(i = 0; i < CALMAN_FILTER_NUMBER_ANGLES; i++)
		 {
			 if(calmanFilter_result->isBreakL3GD20)
			 {
				 K = 1;
			 }
			 else if(calmanFilter_result->isBreakLSM303DLHC)
			 {
				 K = 0;
			 }
			 else
			 {
				 delta = lsm303dlhc_Result_TypeDef->angle[i] - l3gd20_Result_TypeDef->angle[i];
				 if(delta < 0)
				 {
					 delta = -delta;
				 }
				 if(delta <= DELTA_MIN)
				 {
					 K = K_MIN;
				 }

				 else if(delta > DELTA_MIN && delta < DELTA_MAX)
				 {
					 K = (K_MAX - K_MIN) / (DELTA_MAX - DELTA_MIN) * (delta - DELTA_MIN) + K_MIN;
				 }

				 else if(delta > DELTA_MAX)
				 {
					 K = K_MAX;
				 }
			 }
		//K = 1;
			 calmanFilter_result->angle[i] = l3gd20_Result_TypeDef->angle[i] + K * (lsm303dlhc_Result_TypeDef->angle[i] - l3gd20_Result_TypeDef->angle[i]);
		 }
		//K = 1;
	    //xQueueSend(L3GD20_Result_Queue, l3gd20_Result_TypeDef, 0);
	    //xQueueSend(CalmanFilter_ResultAngles_Queue, calmanFilter_result->angle, 0);
		xQueueSend(CalmanFilter_ResultAnglesToL3GD20_Queue, calmanFilter_result->angle, 0);

		l3gd20_Result_isDone = 0;
		lsm303dlhc_Result_isDone = 0;

		xTimerReset(RTOS_tim_startMeasurement, 0);

	 }



}

void CalmanFilter_InitParameters(CalmanFilter_Result_TypeDef *calmanFilter_result)
{
	calmanFilter_result->angle[CALMAN_FILTER_xNumberAxie] = 0;
	calmanFilter_result->angle[CALMAN_FILTER_yNumberAxie] = 0;
	calmanFilter_result->angle[CALMAN_FILTER_zNumberAxie] = 0;

	calmanFilter_result->omega_L3GD20_prev[L3GD20_xNumberAxie] = 0;
	calmanFilter_result->omega_L3GD20_prev[L3GD20_yNumberAxie] = 0;
	calmanFilter_result->omega_L3GD20_prev[L3GD20_zNumberAxie] = 0;

	calmanFilter_result->axieResult_LSM303DLHC_prev[LSM303DLHC_xNumberAxie] = 0;
	calmanFilter_result->axieResult_LSM303DLHC_prev[LSM303DLHC_yNumberAxie] = 0;
	calmanFilter_result->axieResult_LSM303DLHC_prev[LSM303DLHC_zNumberAxie] = 0;

	calmanFilter_result->counterL3GD20_Freeze = 0;
	calmanFilter_result->counterL3GD20_Wait = 0;
	calmanFilter_result->counterLSM303DLHC_Freeze = 0;
	calmanFilter_result->counterLSM303DLHC_Wait = 0;
	calmanFilter_result->isBreakL3GD20 = 0;
	calmanFilter_result->isBreakLSM303DLHC = 0;
}

