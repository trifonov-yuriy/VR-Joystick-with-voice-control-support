#include "LSM303DLHC/LSM303DLHC.h"
//#include "Sensor/sensor.h"
//#include "USART/usart.h"
//#include "TIM7/tim7.h"

extern xSemaphoreHandle        I2C_DMA_Semaphore;
extern xQueueHandle    	       LSM303DLHC_Result_Queue;
extern xSemaphoreHandle        LSM303DLHC_Start_Semaphore;

void check(uint8_t *errorFlag)
{
	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF))
	{
		GPIOE->ODR |= GPIO_Pin_13;
		I2C_GenerateSTOP(I2C1, ENABLE);
		*errorFlag = 1;
	}
}

void generateStop(uint8_t *errorFlag)
{
	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET){};
	I2C_ClearFlag(I2C1, I2C_IT_STOPF);
}

void LSM303DLHC_Task(void *pvParameters)
{

	//I2C_ITConfig(I2C1, I2C_IT_TXI | I2C_IT_TCI | I2C_IT_NACKI | I2C_IT_STOPF, ENABLE);
	DMA_Cmd(DMA1_Channel7, ENABLE);
	uint8_t errorFlag = 0;
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t solvedFlag = 1;
	unsigned int numberAxie = 0;
	LSM303DLHC->axieResultPrev[0] = 0;
	LSM303DLHC->axieResultPrev[1] = 0;
	while(1)
	{
		if(solvedFlag)
		{
			solvedFlag = 0;
			xQueueSemaphoreTake(LSM303DLHC_Start_Semaphore, portMAX_DELAY);
		}
		//vTaskDelay(10);
		errorFlag = 0;
		I2C_connectToSlave_Write(LSM303DLHC->slaveAddresses);
		while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)
		{
			check(&errorFlag);
			if(errorFlag)
			{
				break;
			}
		}
		if(errorFlag)
		{
			vTaskDelay(10);
			continue;
		}

		I2C_SendData(I2C1, LSM303DLHC->addresses[i+j]);

		while(I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET){};

		generateStop(&errorFlag);

		I2C_connectToSlave_Read(LSM303DLHC->slaveAddresses);

		while(I2C_GetFlagStatus(I2C1, I2C_ISR_TC) == RESET)
		{
			check(&errorFlag);
			if(errorFlag)
			{
				break;
			}
		}
		if(errorFlag)
		{
			vTaskDelay(10);
			continue;
		}
		//uint8_t data = I2C_ReceiveData(I2C1);
		generateStop(&errorFlag);


		j++;
		if(j > 1)
		{
			i = i + 2;
			j = 0;
		}

		if(i > 4)
		{
			i = 0;
			j = 0;
			xSemaphoreTake(I2C_DMA_Semaphore, portMAX_DELAY);
			LSM303DLHC_resultAnalyse();
			solvedFlag = 1;
		}

	}

}

void LSM303DLHC_resultAnalyse()
{
	uint8_t j = 0;
	uint8_t i = 0;
	uint8_t counter = 0;
	for(i = 0; i < LSM303DLHC_NUMBER_AXIES; i++)
	{
		LSM303DLHC->result[i] = (LSM303DLHC->receiveData[j] | (LSM303DLHC->receiveData[j + 1] << 8));
		j = j + 2;
		if((LSM303DLHC->result[i] & 0x8000) == 0)
		{
			LSM303DLHC->sign[i] = 0;
		}
		else
		{
			LSM303DLHC->sign[i] = 1;
			LSM303DLHC->result[i] &= 0x7FFF;
			LSM303DLHC->result[i] = 0x8000 - LSM303DLHC->result[i];
		}
		if(LSM303DLHC->result[i] < 0x20)
		{
			LSM303DLHC->result[i] = 0;
		}


		LSM303DLHC->result[i] = LSM303DLHC->result[i] / LSM303DLHC_CORRECT_RESULT;

		LSM303DLHC->axieResult[i] = LSM303DLHC->result[i];


		if(LSM303DLHC->sign[i] == 0)
		{
			//LSM303DLHC->position = LSM303DLHC->result[LSM303DLHC->numberAxie];
			LSM303DLHC->axieResult[i] = -LSM303DLHC->result[i];
		}


		LSM303DLHC->axieResultBuff[i][LSM303DLHC->buffCounter[i]] = LSM303DLHC->axieResult[i];
		LSM303DLHC->buffCounter[i]++;
		if(LSM303DLHC->buffCounter[i] >= LSM303DLHC_NUMBER_AXIES) LSM303DLHC->buffCounter[i] = 0;


		if ((LSM303DLHC->axieResultBuff[i][0] <= LSM303DLHC->axieResultBuff[i][1]) && (LSM303DLHC->axieResultBuff[i][0] <= LSM303DLHC->axieResultBuff[i][2]))
		{
			LSM303DLHC->axieResult[i] = (LSM303DLHC->axieResultBuff[i][1] <= LSM303DLHC->axieResultBuff[i][2]) ? LSM303DLHC->axieResultBuff[i][1] : LSM303DLHC->axieResultBuff[i][2];
		}

		else
		{
		  if ((LSM303DLHC->axieResultBuff[i][1] <= LSM303DLHC->axieResultBuff[i][0]) && (LSM303DLHC->axieResultBuff[i][1] <= LSM303DLHC->axieResultBuff[i][2]))
	      {
			  LSM303DLHC->axieResult[i] = (LSM303DLHC->axieResultBuff[i][0] <= LSM303DLHC->axieResultBuff[i][2]) ? LSM303DLHC->axieResultBuff[i][0] : LSM303DLHC->axieResultBuff[i][2];
	      }
		  else
		  {
			  LSM303DLHC->axieResult[i] = (LSM303DLHC->axieResultBuff[i][0] <= LSM303DLHC->axieResultBuff[i][1]) ? LSM303DLHC->axieResultBuff[i][0] : LSM303DLHC->axieResultBuff[i][1];
		  }
		}


	   // LSM303DLHC->axieResult[i] = LSM303DLHC->axieResultPrev[i] + (LSM303DLHC->axieResult[i] - LSM303DLHC->axieResultPrev[i]) * LSM303DLHC_NOISE_REDUCTION_RATIO;

		//LSM303DLHC->axieResultPrev[i] = LSM303DLHC->axieResult[i];

	}


		if(LSM303DLHC->axieResult[0] == 0)
		{
			LSM303DLHC->axieResult[0] = 1;
		}
		if(LSM303DLHC->axieResult[1] == 1)
		{
			LSM303DLHC->axieResult[1] = 1;
		}
		LSM303DLHC_Result->angle[0] = ( asinf(LSM303DLHC->axieResult[1] /
				sqrtf(powf(LSM303DLHC->axieResult[0], 2) + powf(LSM303DLHC->axieResult[1], 2) +
						powf(LSM303DLHC->axieResult[2], 2))) * 180 / 3.14);



		LSM303DLHC_Result->angle[1] =  ( asinf(LSM303DLHC->axieResult[0] /
						sqrtf(powf(LSM303DLHC->axieResult[0], 2) + powf(LSM303DLHC->axieResult[1], 2) +
								powf(LSM303DLHC->axieResult[2], 2))) * 180 / 3.14);

		//LSM303DLHC->angle[0] = LSM303DLHC->axieResultPrev[0] + (LSM303DLHC->angle[0] - LSM303DLHC->axieResultPrev[0]) * LSM303DLHC_NOISE_REDUCTION_RATIO;
		//LSM303DLHC->angle[1] = LSM303DLHC->axieResultPrev[1] + (LSM303DLHC->angle[1] - LSM303DLHC->axieResultPrev[1]) * LSM303DLHC_NOISE_REDUCTION_RATIO;

		//LSM303DLHC->axieResultPrev[0] = LSM303DLHC->angle[0];
		//LSM303DLHC->axieResultPrev[1] = LSM303DLHC->angle[1];

	     LSM303DLHC_Result->axieResult[LSM303DLHC_xNumberAxie] = abs(LSM303DLHC->axieResult[LSM303DLHC_xNumberAxie]);
		 LSM303DLHC_Result->axieResult[LSM303DLHC_yNumberAxie] = abs(LSM303DLHC->axieResult[LSM303DLHC_yNumberAxie]);
		 LSM303DLHC_Result->axieResult[LSM303DLHC_zNumberAxie] = abs(LSM303DLHC->axieResult[LSM303DLHC_zNumberAxie]);


		xQueueSend(LSM303DLHC_Result_Queue, LSM303DLHC_Result, 0);


}


bool LSM303DLHC_changeNumberAxie()
{
	LSM303DLHC->numberAxie++;
	if(LSM303DLHC->numberAxie > 2)
	{
		LSM303DLHC->numberAxie = 0;
		return false;
	}

	return true;
}

void LSM303DLHC_initParameters()
{
	LSM303DLHC = &lsm303dlhc_typedef;

	LSM303DLHC->receiveData[0] = 0;
	LSM303DLHC->receiveData[1] = 0;

	LSM303DLHC->axieResultPrev[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC->axieResultPrev[LSM303DLHC_yNumberAxie] = 0;
	LSM303DLHC->axieResultPrev[LSM303DLHC_zNumberAxie] = 0;

	LSM303DLHC->axieResultBuff[LSM303DLHC_xNumberAxie][0] = 0;
	LSM303DLHC->axieResultBuff[LSM303DLHC_yNumberAxie][1] = 0;
	LSM303DLHC->axieResultBuff[LSM303DLHC_zNumberAxie][2] = 0;

	LSM303DLHC->buffCounter[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC->buffCounter[LSM303DLHC_yNumberAxie] = 0;
	LSM303DLHC->buffCounter[LSM303DLHC_zNumberAxie] = 0;

	LSM303DLHC->axieResultPrev[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC->axieResultPrev[LSM303DLHC_yNumberAxie] = 0;
	LSM303DLHC->axieResultPrev[LSM303DLHC_zNumberAxie] = 0;

	LSM303DLHC->axieResult[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC->axieResult[LSM303DLHC_yNumberAxie] = 0;
	LSM303DLHC->axieResult[LSM303DLHC_zNumberAxie] = 0;

	LSM303DLHC->sign[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC->sign[LSM303DLHC_yNumberAxie] = 0;
	LSM303DLHC->sign[LSM303DLHC_zNumberAxie] = 0;

	LSM303DLHC->transmitFlagsCounter = 0;

	LSM303DLHC->choiseSensor = 0;

	LSM303DLHC->numberSensor = 0;

	LSM303DLHC->slaveAddresses = LSM303DLHC_LA_SLAVE_ADDR;

	LSM303DLHC->addresses[0] = 0x28; //X_L_A
	LSM303DLHC->addresses[1] = 0x29; //X_H_A
	LSM303DLHC->addresses[2] = 0x2A; //Y_L_A
	LSM303DLHC->addresses[3] = 0x2B; //Y_H_A
	LSM303DLHC->addresses[4] = 0x2C; //Z_L_A
	LSM303DLHC->addresses[5] = 0x2D; //Z_H_A

	LSM303DLHC->result[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC->result[LSM303DLHC_yNumberAxie] = 0;
	LSM303DLHC->result[LSM303DLHC_zNumberAxie] = 0;

	LSM303DLHC->numberAxie = 0;

	LSM303DLHC->angle[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC->angle[LSM303DLHC_yNumberAxie] = 0;


	LSM303DLHC_Result = &lsm303dlhc_Result_TypeDef;
	LSM303DLHC_Result->axieResult[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC_Result->axieResult[LSM303DLHC_yNumberAxie] = 0;
	LSM303DLHC_Result->axieResult[LSM303DLHC_zNumberAxie] = 0;
	LSM303DLHC_Result->angle[LSM303DLHC_xNumberAxie] = 0;
	LSM303DLHC_Result->angle[LSM303DLHC_yNumberAxie] = 0;



}

void LSM303DLHC_changeBit()
{
	LSM303DLHC->numberBit++;
}

bool LSM303DLHC_checkBit()
{
	if(LSM303DLHC->numberBit > 1)
	{
		LSM303DLHC->numberBit = 0;
		return false;
	}
	else
	{
		return true;
	}

}
