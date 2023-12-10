#ifndef LSM303DLHC_H_
#define LSM303DLHC_H_


#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stdbool.h>
#include <math.h>
#include "I2C/i2c.h"
#include <portmacro.h>


#define LSM303DLHC_LA_SLAVE_ADDR   0b00110010
#define LSM303DLHC_MF_SLAVE_ADDR   0b00111100
#define LSM303DLHC_MF_SLAVE_ADDR_W   0b00111100
#define LSM303DLHC_MF_SLAVE_ADDR_R   0b00111010



#define LSM303DLHC_NUMBER_AXIES 	3

#define LSM303DLHC_NUMBER_RESULT    2

#define LSM303DLHC_xNumberAxie      0

#define LSM303DLHC_yNumberAxie      1

#define LSM303DLHC_zNumberAxie      2

#define LSM303DLHC_CORRECT_RESULT   1

#define LSM303DLHC_NOISE_REDUCTION_RATIO 0.1

#define LSM303DLHC_BUFF_SIZE        3

typedef struct
{
	uint8_t receiveData[LSM303DLHC_NUMBER_AXIES * 2];

	float axieResultPrev[LSM303DLHC_NUMBER_AXIES];

	int16_t axieResultBuff[LSM303DLHC_NUMBER_AXIES][LSM303DLHC_BUFF_SIZE];

	uint8_t buffCounter[LSM303DLHC_NUMBER_AXIES];

	int16_t axieResult[LSM303DLHC_NUMBER_AXIES];

	uint16_t result[LSM303DLHC_NUMBER_AXIES];

	uint8_t sign[LSM303DLHC_NUMBER_AXIES];

	uint8_t numberAxie;

	uint8_t transmitFlagsCounter;

	uint8_t choiseSensor; // 0 - Linear acceleration, 1 - Magnetic field

	uint8_t numberBit; // 0 - Low bit, 1 - High bit

	uint8_t addresses[LSM303DLHC_NUMBER_AXIES * 2];

	uint8_t changedAxie;

	uint8_t numberSensor;

	uint8_t slaveAddresses;

	float angle[LSM303DLHC_NUMBER_RESULT];

}LSM303DLHC_TypeDef;


typedef struct
{
	uint16_t axieResult[LSM303DLHC_NUMBER_AXIES]; //Ускорения
	float angle[LSM303DLHC_NUMBER_RESULT]; //Углы от акселерометра


}LSM303DLHC_Result_TypeDef;


LSM303DLHC_TypeDef lsm303dlhc_typedef;
LSM303DLHC_TypeDef *LSM303DLHC;

LSM303DLHC_Result_TypeDef lsm303dlhc_Result_TypeDef;
LSM303DLHC_Result_TypeDef *LSM303DLHC_Result;

void LSM303DLHC_Task(void *pvParameters);

void LSM303DLHC_resultAnalyse();

void LSM303DLHC_initParameters();

void LSM303DLHC_LA_GetXYZ();

bool LSM303DLHC_changeNumberAxie();

void LSM303DLHC_changeBit();

bool LSM303DLHC_checkBit();


#endif
