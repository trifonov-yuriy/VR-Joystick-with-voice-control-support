//#define DEBUG
//#define LSM303DLHC_CORRECT_RESULT   80
//#define LSM303DLHC_CORRECT_RESULT   1

#include <stm32f30x.h>
#include <FreeRTOSConfig.h>
#include <projdefs.h>
#include <portmacro.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <croutine.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <stm32f30x_usart.h>
#include "hw_config.h"

#include "gpio_settings/GPIO.h"


#include "spi/spi.h"
#include "L3GD20/l3gd20.h"
#include "I2C/i2c.h"
#include "LSM303DLHC/lsm303dlhc.h"
#include <stm32f30x_iwdg.h>
#include "IWatchDog/iwatchdog.h"
#include "WWatchDog/wwatchdog.h"
#include "Keys/keys.h"
#include "USART/usart.h"

#include "CalmanFilter/calmanFilter.h"
#include "LCD1602/lcd1602.h"
#include "ADC/adc.h"
#include "Clocking/clocking.h"
#include "Flash/flash.h"
#include "ProgramTimer/programTimer.h"


__IO uint8_t PrevXferComplete = 1;

void Result_Task(void *pvParameters);


void ButtonScan_Task(void *pvParameters);

void IWatchDogReload_Task(void *pvParameters);



xSemaphoreHandle    	SPI_Semaphore;
xSemaphoreHandle 		SPI_DMA_Semaphore;
xSemaphoreHandle        I2C_DMA_Semaphore;
xSemaphoreHandle		Button_Semaphore;
//xSemaphoreHandle        DMA_ADC_IT_HT_Semaphore;
xSemaphoreHandle        DMA_ADC_IT_TC_Semaphore;
xSemaphoreHandle        SettingsSet_Semaphore;
xSemaphoreHandle        L3GD20_Start_Semaphore;
xSemaphoreHandle        LSM303DLHC_Start_Semaphore;
xSemaphoreHandle        UART_Start_Semaphore;

xQueueHandle    	    NumberAxie_Queue;
xQueueHandle    	    L3GD20_Result_Queue;
xQueueHandle    	    LSM303DLHC_Result_Queue;
//xQueueHandle            CalmanFilter_ResultAngles_Queue;
xQueueHandle            CalmanFilter_ResultAnglesToL3GD20_Queue;
xQueueHandle            LCD1602_Angles_Queue;
#ifdef DEBUG
    xQueueHandle    	CalmanFilter_ResultAngles_Queue_DEBUG;
    xQueueHandle        ADC_Result_Queue_DEBUG;
#endif

xTimerHandle RTOS_tim_1;
xTimerHandle RTOS_tim_startMeasurement;

xTaskHandle L3GD20_Task_Handle;
xTaskHandle LSM303DLHC_Task_Handle;
xTaskHandle ButtonScan_Task_Handle;
xTaskHandle Result_Task_Handle;
xTaskHandle ADCResult_Task_Handle;
xTaskHandle UART_Sender_Task_Handle;


USB_DEVICE usb_device;

uint32_t usb_device_readed;

int main(void)
{
	SetClockSettings();
	Choice_USB_Device();

	//Flash_Write(255, 0);

#ifndef DEBUG
	Init_IWatchDog();


	Set_System();

	USB_Interrupts_Config();

    Set_USBClock();

	USB_Init(usb_device);
#endif

	//uint8_t symbols = KEY_W;

	GPIO_init();

	//GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	//TIM7_INIT();

	SPI_INIT();
	L3GD20_initParameters();
	DMA_initChannel2();


	I2C_init();
	LSM303DLHC_initParameters();
	DMA_initChannel7();

	SPI_writeData(0x20, 0x0F); //Измерения по всем трем осям
	SPI_writeData(0x23, 0x30); //Диапазон измерения 2000 гр/сек


	I2C_writeByte(LSM303DLHC_LA_SLAVE_ADDR, 0x20, 0x47); //Измерение относительно осей x,y,z
	I2C_writeByte(LSM303DLHC_LA_SLAVE_ADDR, 0x23, 0x18); //Диапазон измерения +-4g
	I2C_writeByte(LSM303DLHC_LA_SLAVE_ADDR, 0x21, 0x90); //Режим фильтрации верхних частот - normal mode

	//L3GD20_receiveData = xQueueCreate();
	SPI_Semaphore = 		      	     xSemaphoreCreateCounting(10, 0);
	SPI_DMA_Semaphore =           	     xSemaphoreCreateCounting(10, 0);
	I2C_DMA_Semaphore =           	     xSemaphoreCreateCounting(10, 0);
	//DMA_ADC_IT_HT_Semaphore =            xSemaphoreCreateCounting(10, 0);
	DMA_ADC_IT_TC_Semaphore =            xSemaphoreCreateCounting(10, 0);
	SettingsSet_Semaphore =              xSemaphoreCreateCounting(10, 0);
	L3GD20_Start_Semaphore =             xSemaphoreCreateCounting(10, 0);
	LSM303DLHC_Start_Semaphore =         xSemaphoreCreateCounting(10, 0);
	UART_Start_Semaphore =               xSemaphoreCreateCounting(10, 0);

	Button_Semaphore =            	     xSemaphoreCreateCounting(1, 0);
	NumberAxie_Queue =     	             xQueueCreate(1, sizeof(uint8_t));
	L3GD20_Result_Queue =                xQueueCreate(1, sizeof(L3GD20_Result_TypeDef));

	LSM303DLHC_Result_Queue =            xQueueCreate(1, sizeof(LSM303DLHC_Result_TypeDef));
	//CalmanFilter_ResultAngles_Queue =    xQueueCreate(1, sizeof(float[CALMAN_FILTER_NUMBER_ANGLES]));
	CalmanFilter_ResultAnglesToL3GD20_Queue = xQueueCreate(1, sizeof(float[CALMAN_FILTER_NUMBER_ANGLES]));
	//LCD1602_Queue  =                     xQueueCreate(10, sizeof(LCD1602_Data_TypeDef));
	LCD1602_Angles_Queue  =                xQueueCreate(1, sizeof(LCD1602_Angles_TypeDef));

#ifdef DEBUG
	CalmanFilter_ResultAngles_Queue_DEBUG  =   xQueueCreate(1, sizeof(float[L3GD20_NUMBER_ANGLES]));
	ADC_Result_Queue_DEBUG    =                xQueueCreate(1, sizeof(uint16_t));
	//USART_init();
	//xTaskCreate(USARTSend_Task, (const char *) "USARTSend_Task", 256, NULL, 3, (xTaskHandle) NULL);
#endif

	RTOS_tim_1 = xTimerCreate("TIM1", 2000, pdFALSE, 0, TimerFunction_one_shot);
	RTOS_tim_startMeasurement = xTimerCreate("StartMeasurement", 10, pdFALSE, 0, StartMeasurement);
	xTimerStart(RTOS_tim_1, 0);
	xTimerStart(RTOS_tim_startMeasurement, 0);

	xTaskCreate(L3GD20_Task, (const char *) "L3GD20_Task", 256, NULL, 3, &L3GD20_Task_Handle);
	xTaskCreate(LSM303DLHC_Task, (const char *) "LSM303DLHC_Task", 256, NULL, 3, &LSM303DLHC_Task_Handle);
	xTaskCreate(ButtonScan_Task, (const char *) "ButtonScan_Task", 256, NULL, 4, &ButtonScan_Task_Handle);
	xTaskCreate(Result_Task, (const char *) "Result_Task", 256, NULL, 3, &Result_Task_Handle);
	xTaskCreate(LCD1602_Task, (const char *) "LCD1602_Task", 256, NULL, 3, (xTaskHandle) NULL);
	xTaskCreate(ADCResult_Task, (const char *) "ADCResult_Task", 256, NULL, 3, &ADCResult_Task_Handle);
	//xTaskCreate(UART_Sender_Task, (const char *) "UART_Sender_Task", 256, NULL, 3, &UART_Sender_Task_Handle);

//	xTaskCreate(Task_2, (const char *) "Task_2", 256, NULL, 2, (xTaskHandle) NULL);
	//xTaskCreate(CalmanFilterTask, (const char *) "CalmanFilterTask", 256, NULL, 3, (xTaskHandle) NULL);
#ifndef DEBUG
	xTaskCreate(IWatchDogReload_Task, (const char *) "IWatchDogReload_Task", 256, NULL, 3, (xTaskHandle) NULL);
#endif

	vTaskStartScheduler();

	//GPIO_init();
	//ADC_init();
	//TIM6_init();
	//Speex_Init();


    while(1)
    {
    	//EncodingVoice();
    }
}



void Choice_USB_Device()
{
	usb_device_readed = FLASH_Read(255);
	usb_device = usb_device_readed;
}

void ButtonScan_Task(void *pvParameters)
{
	unsigned int counter = 0;
	while(1)
	{
		xQueueSendToBack(NumberAxie_Queue, &counter, 0);
		xSemaphoreTake(Button_Semaphore, portMAX_DELAY);
		vTaskDelay(10);
		counter++;
		if(counter > 5) counter = 0;

	}

}

void ResultAnalyze(uint8_t *symbols, int16_t *l3gd20_result, float *calmanFilter_resultAngles)
{
	if(l3gd20_result[L3GD20_xNumberAxie] > 5 && calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie] > 0)
	{
		symbols[0] = KEY_A;
	}
	else if(l3gd20_result[L3GD20_xNumberAxie] < -5 && calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie] < 0)
	{
		symbols[0] = KEY_D;
	}

	else
	{
		symbols[0] = KEY_NONE;
	}

	if(calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie] > 20)
	{
		symbols[0] = KEY_A;
	}
	else if(calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie] < -20)
	{
		symbols[0] = KEY_D;
	}

	if(calmanFilter_resultAngles[CALMAN_FILTER_yNumberAxie] > 10)
	{
		symbols[1] = KEY_W;
	}
	else if(calmanFilter_resultAngles[CALMAN_FILTER_yNumberAxie] < -10)
	{
		symbols[1] = KEY_S;
	}
	else
	{
		symbols[1] = KEY_NONE;
	}

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		symbols[2] = KEY_X;
	}
	else
	{
		symbols[2] = KEY_NONE;
	}

}

void ResultAnalyze_GamePad(uint8_t *buf, float *calmanFilter_resultAngles)
{
	if(calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie] > 0)
	{
		buf[1] = 255 - (uint8_t) calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie] * 127 / 90;
	}

	else if(calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie] < 0)
	{
		buf[1] = (uint8_t) -(calmanFilter_resultAngles[CALMAN_FILTER_xNumberAxie]) * 127 / 90;
	}

	if(calmanFilter_resultAngles[CALMAN_FILTER_yNumberAxie] > 0)
	{
		buf[0] = (uint8_t) calmanFilter_resultAngles[CALMAN_FILTER_yNumberAxie] * 127 / 90;
	}

	else if(calmanFilter_resultAngles[CALMAN_FILTER_yNumberAxie] < 0)
	{
		buf[0] = 255 - (uint8_t) (-calmanFilter_resultAngles[CALMAN_FILTER_yNumberAxie]) * 127 / 90;
	}

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		buf[3] = 1;
	}
	else
	{
		buf[3] = 0;
	}


}

void IWatchDogReload_Task(void *pvParameters)
{
	while(1)
	{
		IWDG_ReloadCounter();
	}
}

void Result_Task(void *pvParameters)
{
	uint8_t numberAxie = 0;
	uint8_t symbols[3] = {0, 0, 0};
	uint8_t buff[4] = {0, 0, 0, 0};
	uint8_t is_initWWatchDog = 0;
	uint16_t wWatchDogCounter = 0;
	uint32_t counterSettings = 0;
	CalmanFilter_Result_TypeDef calmanFilter_result; //Результат работы с фильтром

    L3GD20_Result_TypeDef l3gd20_Result_TypeDef;
    LSM303DLHC_Result_TypeDef lsm303dlhc_Result_TypeDef;
    CalmanFilter_InitParameters(&calmanFilter_result);
    LCD1602_Angles_TypeDef lcd1602_angles_typedef;


	while(1)
	{
		CalmanFilter_Result(&l3gd20_Result_TypeDef, &lsm303dlhc_Result_TypeDef, &calmanFilter_result);

		lcd1602_angles_typedef.L3GD20_Angles[0] = (int16_t) l3gd20_Result_TypeDef.angle[0];
		lcd1602_angles_typedef.L3GD20_Angles[1] = (int16_t) l3gd20_Result_TypeDef.angle[1];
		lcd1602_angles_typedef.LSM303DLHC_Angles[0] = (int16_t) lsm303dlhc_Result_TypeDef.angle[0];
		lcd1602_angles_typedef.LSM303DLHC_Angles[1] = (int16_t) lsm303dlhc_Result_TypeDef.angle[1];
		lcd1602_angles_typedef.Calman_Angles[0] = (int16_t) calmanFilter_result.angle[0];
		lcd1602_angles_typedef.Calman_Angles[1] = (int16_t) calmanFilter_result.angle[1];

		xQueueSend(LCD1602_Angles_Queue, &lcd1602_angles_typedef, 0);

#ifndef DEBUG
	      if (bDeviceState == CONFIGURED && PrevXferComplete)
		  {
	        	if(is_initWWatchDog == 0)
	        	{
	        		//Init_WWatchDog();
	        		is_initWWatchDog = 1;
	        	}
	        	switch(usb_device)
	        	{
	        	case EMULATOR_KEYBOARD:
	        	{
	        		ResultAnalyze(symbols, l3gd20_Result_TypeDef.omega, calmanFilter_result.angle);
	        		Joystick_Send(symbols);
	        	} break;
	        	case GAMEPAD:
	        	{
	        		ResultAnalyze_GamePad(buff, calmanFilter_result.angle);
	        		GamePad_Send(buff);
	        	} break;
	        	}
			  	//GPIOE->ODR = (1<<12);
			  	//Joystick_Send(KEY_H);
			  	//Joystick_Send(KEY_NONE);wwwwwwwwwwwwwwwwww
			 	//Joystick_Send(symbols);
			 	wWatchDogCounter = 0;
			}
	    	 wWatchDogCounter++;
	    	 if(wWatchDogCounter < WWATCH_DOG_TIMER_LIMIT)
	    	 {
	    		// WWDG_SetCounter(WWATCH_DOG_TIMER);
	    	 }
#endif


#ifdef DEBUG
			xQueueSend(CalmanFilter_ResultAngles_Queue_DEBUG, calmanFilter_result.angle, 0);
#endif

			xQueueReceive(NumberAxie_Queue, &numberAxie, 0);

			if(numberAxie == 0)
			{
				L3GD20_LEDS(l3gd20_Result_TypeDef.omega[L3GD20_xNumberAxie]);
			}
			else if(numberAxie == 1)
			{
				L3GD20_LEDS(l3gd20_Result_TypeDef.omega[L3GD20_yNumberAxie]);
			}
			else if(numberAxie == 2)
			{
				L3GD20_LEDS(l3gd20_Result_TypeDef.omega[L3GD20_zNumberAxie]);
			}
			else if(numberAxie == 3)
			{
				LSM303DLHC_LEDS(lsm303dlhc_Result_TypeDef.axieResult[LSM303DLHC_xNumberAxie]);
			}
			else if(numberAxie == 4)
			{
				LSM303DLHC_LEDS(lsm303dlhc_Result_TypeDef.axieResult[LSM303DLHC_yNumberAxie]);
			}
			else if(numberAxie == 5)
			{
				LSM303DLHC_LEDS(lsm303dlhc_Result_TypeDef.axieResult[LSM303DLHC_zNumberAxie]);
			}
		IWDG_ReloadCounter();
   }

}


void L3GD20_LEDS(int16_t omega)
{
	GPIOE->ODR = 0x0000;
	if((omega >= 0 && omega < 50) ||
		(omega <= 0 && omega > -50))
	{
		GPIOE->ODR |= GPIO_Pin_11;
	}
	else if(omega > 50 && omega < 100)
	{
		GPIOE->ODR |= GPIO_Pin_12;
	}
	else if(omega > 100 && omega < 150)
	{
		GPIOE->ODR |= GPIO_Pin_13;
	}
	else if(omega > 150 && omega)
	{
		GPIOE->ODR |= GPIO_Pin_14;
	}
	else if(omega < -50 && omega > -100)
	{
		GPIOE->ODR |= GPIO_Pin_10;
	}
	else if(omega < -100 && omega > -150)
	{
		GPIOE->ODR |= GPIO_Pin_9;
	}
	else if(omega < -150)
	{
		GPIOE->ODR |= GPIO_Pin_8;
	}
}

void LSM303DLHC_LEDS(uint16_t resultFromSensor)
{
	GPIO_Write(GPIOE, 0x0000);
	if((resultFromSensor) >= 100 && (resultFromSensor) < 110)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
	}
	else if((resultFromSensor) >= 110 && (resultFromSensor) < 120)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_12);
	}
	else if((resultFromSensor) >= 120 && (resultFromSensor) < 130)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_13);
	}
	else if((resultFromSensor) >= 130)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_14);
	}
	else if((resultFromSensor) >= 90 && (resultFromSensor) < 100)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_10);
	}
	else if((resultFromSensor) >= 80 && (resultFromSensor) < 90)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_9);
	}
	else if((resultFromSensor) >= 0 && (resultFromSensor) < 80)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_8);
	}
}




void vApplicationIdleHook(){}
void vApplicationMallocFailedHook()
{
for ( ;; );
}
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
( void ) pcTaskName;
( void ) pxTask;
for ( ;; );
}
void vApplicationTickHook(){}

