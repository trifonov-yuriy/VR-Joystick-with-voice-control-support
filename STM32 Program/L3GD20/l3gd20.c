#include "L3GD20/L3GD20.h"
//#include "Sensor/sensor.h"
//#include "USART/usart.h"
//#include "SysTick/systick.h"
#include <math.h>
#include "LSM303DLHC/lsm303dlhc.h"

extern xSemaphoreHandle    	    SPI_Semaphore;
extern xSemaphoreHandle 		SPI_DMA_Semaphore;
extern xQueueHandle    	        L3GD20_Result_Queue;
extern xQueueHandle             CalmanFilter_ResultAnglesToL3GD20_Queue;
extern xSemaphoreHandle        L3GD20_Start_Semaphore;

//double timer = 0;
void L3GD20_Task(void *pvParameters)
{
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	DMA_Cmd(DMA1_Channel2, ENABLE);
	uint8_t i = 0;
	unsigned int numberAxie = 0;

	//L3GD20->angle[0] = 40;
	//L3GD20->angle[1] = 45;
	while(1)
	{

		//vTaskDelay(10);
		//TIM7->CNT = 0;
		xQueueSemaphoreTake(L3GD20_Start_Semaphore, portMAX_DELAY);

		for(i = 0; i < L3GD20_NUMBER_AXIES; i++)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);
			//SPI_SendData8(SPI1, L3GD20->addresses[L3GD20->numberAxie]);
			SPI_SendData8(SPI1, L3GD20->addresses[i]);
			xQueueSemaphoreTake(SPI_Semaphore, portMAX_DELAY);
			SPI_SendData8(SPI1, 0x00);
			xQueueSemaphoreTake(SPI_Semaphore, portMAX_DELAY);
			SPI_SendData8(SPI1, 0x00);
			xQueueSemaphoreTake(SPI_Semaphore, portMAX_DELAY);
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
		}

		xQueueSemaphoreTake(SPI_DMA_Semaphore, portMAX_DELAY);
		//timer = TIM7->CNT * 0.000002;
		L3GD20_resultAnalyse();


		/*xQueueReceive(L3GD20_NumberAxie_Queue, &numberAxie, 0);

		if(numberAxie == 0)
		{
			L3GD20_resultanfalyse(L3GD20_xNumberAxie);
		}
		else if(numberAxie == 1)
		{
			L3GD20_resultanfalyse(L3GD20_yNumberAxie);
		}
		else if(numberAxie == 2)
		{
			L3GD20_resultanfalyse(L3GD20_zNumberAxie);
		}
		else
		{
			L3GD20_resultanfalyse(100);
		}*/



		//GPIOE->ODR ^= GPIO_Pin_10;
		//vTaskDelay(100);
	}
}

void L3GD20_solveAngles()
{
    float betta_0 = 0.001f;

    float gamma = 0.001f;

    float fi = 0.001f;

    float psi = 0.001f;

    float z_k = 0.001f;
    //float x_k = 0.001f;

    float z_t = 0.001f;
   // float y_t = 0.001f;
    float value = 0.001f;
    float alpha = L3GD20_Result->angle[L3GD20_xNumberAxie];
    float betta = L3GD20_Result->angle[L3GD20_yNumberAxie];

    if(alpha == 0)
    {
    	alpha = 0.001;
    }

    if(betta == 0)
    {
    	betta = 0.001;
    }


    //L3GD20->omega[L3GD20_xNumberAxie] = 1000;
   // L3GD20->omega[L3GD20_yNumberAxie] = -1000;
    z_k = RADIUS * sinf(alpha * RADIANS);
    z_t = RADIUS * sinf(betta * RADIANS);
    //if(L3GD20->omega[L3GD20_xNumberAxie] != 0)

    {
       value = z_k / (RADIUS * cosf(betta * RADIANS));
       if(value > 1)
       {
    	   value = 1;
       }
       else if(value < -1)
       {
    	   value = -1;
       }
       fi = asinf (value) * DEGREES;
       z_k = RADIUS * sinf((fi + L3GD20_Result->omega[L3GD20_xNumberAxie] * L3GD20_TIMEINTERVAL) * RADIANS) * cosf(betta * RADIANS);
       //x_k = RADIUS * cosf((fi + L3GD20->omega[L3GD20_xNumberAxie] * L3GD20_TIMEINTERVAL) * RADIANS);
       alpha = asinf(z_k / RADIUS) * DEGREES;
     //  if(z_k >0 && x_k < 0)
      // {
    //	   alpha = 90 + (90 - alpha);
     //  }

    }


   // if(L3GD20->omega[L3GD20_yNumberAxie] != 0)
    {
       value = z_t / (RADIUS * cosf(alpha * RADIANS));
       if(value > 1)
       {
    	   value = 1;
       }
       else if(value < -1)
       {
    	   value = -1;
       }
       psi = asinf (value) * DEGREES;
       z_t = RADIUS * sinf((psi + L3GD20_Result->omega[L3GD20_yNumberAxie] * L3GD20_TIMEINTERVAL) * RADIANS) * cosf(alpha * RADIANS);
      // y_t = RADIUS * cosf((psi + L3GD20->omega[L3GD20_yNumberAxie] * L3GD20_TIMEINTERVAL) * RADIANS);

       betta = asinf(z_t / RADIUS) * DEGREES;
      // if(z_t > 0 && y_t < 0)
      // {
    //	   betta = 90 + (90 - betta);
     //  }
    }


   // if(L3GD20->omega[L3GD20_zNumberAxie] != 0)

    {


    	gamma = atanf(z_k/z_t) * DEGREES;
    	//gamma = atanf(alpha/betta) * DEGREES;

    	value = z_k / (RADIUS * sinf(gamma * RADIANS));
    	if(value > 1)
    	{
    		value = 1;
    	}
    	else if(value < -1)
    	{
    		value = -1;
    	}

    	betta_0 = asinf(value) * DEGREES;

    	z_k = RADIUS * sinf((gamma - L3GD20_Result->omega[L3GD20_zNumberAxie] * L3GD20_TIMEINTERVAL ) * RADIANS) * sinf(betta_0 * RADIANS);
    	z_t = RADIUS * cosf((gamma - L3GD20_Result->omega[L3GD20_zNumberAxie] * L3GD20_TIMEINTERVAL) * RADIANS) * sinf(betta_0 * RADIANS);

    	alpha = asinf(z_k / RADIUS) * DEGREES;
    	betta = asinf(z_t / RADIUS) * DEGREES;

    }

    /*
    if(alpha == 0)
    {
    	alpha = 0.001;
    }

    if(betta == 0)
    {
    	betta = 0.001;
    }
*/


    L3GD20_Result->angle[L3GD20_xNumberAxie] = alpha;
    L3GD20_Result->angle[L3GD20_yNumberAxie] = betta;
}


void L3GD20_resultAnalyse()
{
	uint8_t j = 0;
	uint8_t i = 0;
	uint16_t result = 0;
	uint8_t sign = 0;

	for(i = 0; i < L3GD20_NUMBER_AXIES; i++)
	{
		result = (L3GD20->receiveData[j + 1] | (L3GD20->receiveData[j + 2] << 8)) + L3GD20->correct[i];

		j = j + 3;

		if((result & 0x8000) == 0)
		{
			sign = 0;
		}
		else
		{
			sign = 1;
			result &= 0x7FFF;
			result = 0x8000 - result;
		}

		if(result < 0x20)//!!!ÂÅÐÍÓÒÜif(result < 0x40)
		{
			result = 0;
			sign = 0;
		}
		if(i == 2 && result < 0x20) //!!!ÂÅÐÍÓÒÜif(i == 2 && result < 0x40)
		{
			result = 0;
			sign = 0;
		}
		L3GD20->result[i] = result;
		if(sign == 0)
		{
			L3GD20_Result->omega[i] = -result;
		}
		else
		{
			L3GD20_Result->omega[i] = result;
		}

		L3GD20_Result->omega[i] = L3GD20_Result->omega[i] * 2000 / 32767;//!!!ÂÅÐÍÓÒÜL3GD20->omega[i] = L3GD20->omega[i] * 2000 / 32767;

	}


	xQueueReceive(CalmanFilter_ResultAnglesToL3GD20_Queue, L3GD20_Result->angle, 0);
	//xQueueReceive(L3GD20_Result_Queue, L3GD20_Result, 0);

	L3GD20_solveAngles();

	xQueueSend(L3GD20_Result_Queue, L3GD20_Result, 0);


}



void L3GD20_initParameters()
{
	L3GD20 = &l3gd20_typedef;

	L3GD20->addresses[L3GD20_xNumberAxie] = 0xE8; //OUT_X_L
	L3GD20->addresses[L3GD20_yNumberAxie] = 0xEA; //OUT_Y_L
	L3GD20->addresses[L3GD20_zNumberAxie] = 0xEC; //OUT_Z_L

	L3GD20->receiveData[0] =  0;
	L3GD20->receiveData[1] =  0;
	L3GD20->receiveData[2] =  0;

	L3GD20->correct[L3GD20_xNumberAxie] = 10;
	L3GD20->correct[L3GD20_yNumberAxie] = 6;
	L3GD20->correct[L3GD20_zNumberAxie] = -7;

	L3GD20->result[L3GD20_xNumberAxie] = 0;
	L3GD20->result[L3GD20_yNumberAxie] = 0;
	L3GD20->result[L3GD20_zNumberAxie] = 0;

	L3GD20->sign[L3GD20_xNumberAxie] = 0;
	L3GD20->sign[L3GD20_yNumberAxie] = 0;
	L3GD20->sign[L3GD20_zNumberAxie] = 0;

	L3GD20->numberAxie = 0;


	L3GD20_Result = &l3gd20_Result_TypeDef;
	L3GD20_Result->omega[L3GD20_xNumberAxie] = 0;
	L3GD20_Result->omega[L3GD20_yNumberAxie] = 0;
	L3GD20_Result->omega[L3GD20_zNumberAxie] = 0;
	L3GD20_Result->angle[L3GD20_xNumberAxie] = 0;
	L3GD20_Result->angle[L3GD20_yNumberAxie] = 0;
}


void L3GD20_changeNumberAxie()
{
	L3GD20->numberAxie++;
	if(L3GD20->numberAxie > 2)
	{
		L3GD20->numberAxie = 0;
	}
}
