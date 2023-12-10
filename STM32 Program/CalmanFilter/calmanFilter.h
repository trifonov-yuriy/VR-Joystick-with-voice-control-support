#ifndef CalmanFilter_H_
#define CalmanFilter_H_

#include "LSM303DLHC/lsm303dlhc.h"
#include "L3GD20/l3gd20.h"
#include <FreeRTOSConfig.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <timers.h>

#define CALMAN_FILTER_NUMBER_ANGLES   2
#define CALMAN_FILTER_ALPHA           0
#define CALMAN_FILTER_BETTA           1
#define CALMAN_FILTER_xNumberAxie     0
#define CALMAN_FILTER_yNumberAxie     1
#define CALMAN_FILTER_zNumberAxie     2
#define K_MIN                         0.1f
#define K_MAX                         0.9f
#define DELTA_MIN                     2
#define DELTA_MAX                     30
#define COUNTER_LIMIT                 8000

typedef struct
{
	float angle[CALMAN_FILTER_NUMBER_ANGLES]; //Углы от гироскопа
	int16_t omega_L3GD20_prev[L3GD20_NUMBER_AXIES]; //Предыдущие данные с гироскопа
	uint16_t axieResult_LSM303DLHC_prev[LSM303DLHC_NUMBER_AXIES]; //Предыдущие данные с акселлерометра
	uint8_t isBreakL3GD20;
	uint8_t isBreakLSM303DLHC;
	uint16_t counterL3GD20_Wait;
	uint16_t counterL3GD20_Freeze;
	uint16_t counterLSM303DLHC_Wait;
	uint16_t counterLSM303DLHC_Freeze;
} CalmanFilter_Result_TypeDef;

void CalmanFilterTask(void *pvParameters);
void CalmanFilter_Result(L3GD20_Result_TypeDef *l3gd20_Result_TypeDef, LSM303DLHC_Result_TypeDef *lsm303dlhc_Result_TypeDef, CalmanFilter_Result_TypeDef *calmanFilter_result);
void CalmanFilter_InitParameters(CalmanFilter_Result_TypeDef *calmanFilter_result);



#endif
