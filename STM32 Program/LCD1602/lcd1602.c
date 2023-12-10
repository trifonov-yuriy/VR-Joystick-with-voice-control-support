#include "LCD1602/lcd1602.h"
#include <stdio.h>

extern xSemaphoreHandle        SettingsSet_Semaphore;
extern xQueueHandle            LCD1602_Angles_Queue;

extern xTaskHandle L3GD20_Task_Handle;
extern xTaskHandle LSM303DLHC_Task_Handle;
extern xTaskHandle ButtonScan_Task_Handle;
extern xTaskHandle Result_Task_Handle;
extern xTaskHandle ADCResult_Task_Handle;
extern xTaskHandle UART_Sender_Task_Handle;


void LCD1602_Task(void *pvParameters)
{
	GPIO_lcd1602_init();
	LCD1602_Init();
	uint8_t settings = 0;
	uint8_t deviceNumber = 0;
	LCD1602_Angles_TypeDef lcd1602_angles_typedef;
	char dataToLED[10];

	while(1)
	{

		if (xQueueSemaphoreTake(SettingsSet_Semaphore, 0) == pdTRUE)
		{
			vTaskDelay(1000);
			vTaskDelete(L3GD20_Task_Handle);
			vTaskDelete(LSM303DLHC_Task_Handle);
			vTaskDelete(ButtonScan_Task_Handle);
			vTaskDelete(Result_Task_Handle);
			vTaskDelete(ADCResult_Task_Handle);
			vTaskDelete(UART_Sender_Task_Handle);

			IWatchDog_Change();

			LCD1602_StrLcd("Choose device:");
			LCD1602_SendCommand(0xc0);	/* Переход на следующую строку */
			LCD1602_StrLcd("Keyboard emul");
			settings = 1;

		}

		if(settings == 1)
		{
			IWDG_ReloadCounter();
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			{
				vTaskDelay(1000);
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
				{
					LCD1602_SendCommand(0x01);	/* Clear display screen */
					deviceNumber++;
					if(deviceNumber > 1)
					{
						deviceNumber = 0;
					}

					switch (deviceNumber)
					{
						case 0:
						{
							LCD1602_StrLcd("Choose device:");
							LCD1602_SendCommand(0xc0);	/* Переход на следующую строку */
							LCD1602_StrLcd("Keyboard emul");
						}break;

						case 1:
						{
							LCD1602_StrLcd("Choose device:");
							LCD1602_SendCommand(0xc0);	/* Переход на следующую строку */
							LCD1602_StrLcd("GamePad");
						} break;

					}
				}
				else
				{
					LCD1602_SendCommand(0x01);	/* Clear display screen */
					LCD1602_StrLcd("Device's choosed");
					Flash_Write(255, deviceNumber);
					vTaskDelay(1000);
				}
			}

		}

		else
		{
			xQueueReceive(LCD1602_Angles_Queue, &lcd1602_angles_typedef, 0);
			//LCD1602_SendCommand(0x01);	/* Clear display screen */
			sprintf(dataToLED, "%d", lcd1602_angles_typedef.L3GD20_Angles[0]);
			LCD1602_Setpos(0, 0);
			LCD1602_StrLcd("   ");
			LCD1602_Setpos(0, 0);
			LCD1602_StrLcd(dataToLED);

			sprintf(dataToLED, "%d", lcd1602_angles_typedef.LSM303DLHC_Angles[0]);
			LCD1602_Setpos(6, 0);
			LCD1602_StrLcd("   ");
			LCD1602_Setpos(6, 0);
			LCD1602_StrLcd(dataToLED);

			sprintf(dataToLED, "%d", lcd1602_angles_typedef.Calman_Angles[0]);
			LCD1602_Setpos(12, 0);
			LCD1602_StrLcd("   ");
			LCD1602_Setpos(12, 0);
			LCD1602_StrLcd(dataToLED);

			sprintf(dataToLED, "%d", lcd1602_angles_typedef.L3GD20_Angles[1]);
			LCD1602_Setpos(0, 1);
			LCD1602_StrLcd("   ");
			LCD1602_Setpos(0, 1);
			LCD1602_StrLcd(dataToLED);

			sprintf(dataToLED, "%d", lcd1602_angles_typedef.LSM303DLHC_Angles[1]);
			LCD1602_Setpos(6, 1);
			LCD1602_StrLcd("   ");
			LCD1602_Setpos(6, 1);
			LCD1602_StrLcd(dataToLED);

			sprintf(dataToLED, "%d", lcd1602_angles_typedef.Calman_Angles[1]);
			LCD1602_Setpos(12, 1);
			LCD1602_StrLcd("   ");
			LCD1602_Setpos(12, 1);
			LCD1602_StrLcd(dataToLED);
		}

		//LCD1602_SendCommand(0xc0);	/* Переход на следующую строку */
		//LCD1602_StrLcd("meaning Lenochka");
	}
}

void LCD1602_Init()
{
	vTaskDelay(20); //1. Необходимо подождать не менее 15 мс, время инициализации включения питания для LCD1602.

	GPIO_ResetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(0b00000011);
	vTaskDelay(4);

	GPIO_ResetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(0b00000011);
	vTaskDelay(1);

	GPIO_ResetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(0b00000011);
	vTaskDelay(1);

	GPIO_ResetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(0b00000010);
	vTaskDelay(1);


	LCD1602_SendCommand(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
	vTaskDelay(1);
	LCD1602_SendCommand(0x0c);	/* Display on cursor off */
	vTaskDelay(1);
	LCD1602_SendCommand(0x06);	/* Increment cursor (shift cursor to right) */
	vTaskDelay(1);
	LCD1602_SendCommand(0x01);	/* Clear display screen */


	//LCD1602_SendData(0b00110100);
	//LCD1602_SendCommand(0x01);	/* Clear display screen */

	//LCD1602_SendData('A');



}

void LCD1602_StrLcd (char str[])
{
	uint8_t i = 0;
	for(i=0; str[i]!=0; i++)
	{
	   LCD1602_SendData(str[i]);
	}
}

void LCD1602_Setpos(unsigned char x, unsigned y)
{
  char adress;

  adress= (0x40*y+x) | 0b10000000;

  LCD1602_SendCommand(adress);

}

void LCD1602_SendHalfByte(unsigned char halfByte)
{
	GPIO_ResetBits(GPIOC, LCD1602_DATA_PINS);
	GPIO_SetBits(GPIOC, LCD1602_E_PIN);
	GPIO_SetBits(GPIOC, halfByte);
	vTaskDelay(1);
	GPIO_ResetBits(GPIOC, LCD1602_E_PIN);
	vTaskDelay(1);
	GPIOC->ODR |= LCD1602_PINS;

}

void LCD1602_SendCommand(unsigned char command)
{
	GPIO_ResetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(command >> 4);
	vTaskDelay(1);


	GPIO_ResetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(command & 0x0F);

	//vTaskDelay(1);
}

void LCD1602_SendData(unsigned char data)
{
	GPIO_SetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(data >> 4);
	vTaskDelay(1);

	GPIO_SetBits(GPIOC, LCD1602_RS_PIN);
	LCD1602_SendHalfByte(data & 0x0F);
	//vTaskDelay(1);
}


