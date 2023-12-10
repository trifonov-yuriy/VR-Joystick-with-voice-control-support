#ifndef LCD1602_H_
#define LCD1602_H_

#include "gpio_settings/GPIO.h"
#include <portmacro.h>
#include <task.h>
#include "Flash/flash.h"
#include "IWatchDog/iwatchdog.h"

typedef enum
{
    L3GD20_Sensor = 1,
    LSM303DLHC_Sensor = 2,
    CalmanFilter_Sensor = 3
} Sensor_TypeDef;

typedef struct
{
	Sensor_TypeDef sensor_typeDef;
	float data;

} LCD1602_Data_TypeDef;

typedef struct
{
   int16_t L3GD20_Angles[2];
   int16_t LSM303DLHC_Angles[2];
   int16_t Calman_Angles[2];
} LCD1602_Angles_TypeDef;

void LCD1602_Task(void *pvParameters);

void LCD1602_Init();

void LCD1602_SendCommand(unsigned char command);

void LCD1602_SendHalfByte(unsigned char halfByte);

void LCD1602_SendData(unsigned char data);


#endif

