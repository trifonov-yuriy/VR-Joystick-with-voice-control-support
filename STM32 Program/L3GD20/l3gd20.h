#ifndef L3GD20_H_
#define L3GD20_H_

#include "DMA/dma.h"
#include <stm32f30x_spi.h>
#include <stdbool.h>





#define L3GD20_NUMBER_AXIES 	3
#define L3GD20_NUMBER_ANGLES 	2
#define L3GD20_xNumberAxie      0
#define L3GD20_yNumberAxie      1
#define L3GD20_zNumberAxie      2

#define RADIANS                 0.01745329
#define DEGREES                 57.29577951

#define RADIUS                  10

#define L3GD20_TIMEINTERVAL     0.0015

typedef struct
{
	uint8_t receiveData[DMA_BUFF_SIZE];// {0, 0, 0};
	uint16_t result[L3GD20_NUMBER_AXIES];
	uint8_t numberAxie;

	uint8_t sign[L3GD20_NUMBER_AXIES];
	int16_t correct[L3GD20_NUMBER_AXIES];

	uint8_t addresses[L3GD20_NUMBER_AXIES];

} L3GD20_TypeDef;


typedef struct
{
	int16_t omega[L3GD20_NUMBER_AXIES]; //”гловые скорости
	float angle[L3GD20_NUMBER_ANGLES]; //”глы от гироскопа
} L3GD20_Result_TypeDef;


L3GD20_TypeDef l3gd20_typedef;
L3GD20_TypeDef *L3GD20;

L3GD20_Result_TypeDef l3gd20_Result_TypeDef;
L3GD20_Result_TypeDef *L3GD20_Result;

void L3GD20_solveAngles();

void L3GD20_Task(void *pvParameters);

void L3GD20_resultAnalyse();

void L3GD20_initParameters();

void L3GD20_changeNumberAxie();


#endif
