#include "I2C/i2c.h"
#include "Sensor/sensor.h"
//#include "SysTick/systick.h"
extern xSemaphoreHandle I2C_DMA_Semaphore;

void I2C_initParameters()
{
	I2C_adres = 0;
	I2C_data = 0;
	I2C_numberLed = 0;


	I2C_flag_adres = 0;
	I2C_flag_data = 0;

	I2C_startReceiveFlag = false;
	I2C_startTransmitFlag = false;
	I2C_sendDataFlag = true;
	I2C_stopFlag = false;

	I2C_numberAxie = 0;
}

void I2C_resetParameters()
{
	I2C_startReceiveFlag = false;
	I2C_startTransmitFlag = false;
	I2C_sendDataFlag = true;
	I2C_stopFlag = false;
}

void I2C_init()
{
	I2C_interrupt_Config();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_I2C1;
	GPIO_I2C1.GPIO_Pin = I2C1_SCL | I2C1_SDA;
	GPIO_I2C1.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_I2C1.GPIO_Mode = GPIO_Mode_AF;
	GPIO_I2C1.GPIO_OType = GPIO_OType_OD;
	GPIO_I2C1.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C1_PORT, &GPIO_I2C1);

	GPIO_PinAFConfig(I2C1_PORT, GPIO_PinSource6, GPIO_AF_4);
	GPIO_PinAFConfig(I2C1_PORT, GPIO_PinSource7, GPIO_AF_4);

	I2C_InitTypeDef I2C1_Struct;

	I2C_DeInit(I2C1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
	I2C1_Struct.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C1_Struct.I2C_Timing = 0xB042181C;
	I2C1_Struct.I2C_OwnAddress1 = I2C_MASTER_ADDR;
	I2C1_Struct.I2C_Ack = I2C_Ack_Enable;
	I2C1_Struct.I2C_Mode = I2C_Mode_I2C;
	I2C1_Struct.I2C_DigitalFilter = 0;
	I2C1_Struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C1_Struct);
   // uint8_t scldel = 4;
   // uint8_t sdadel = 5;
   // I2C1->TIMINGR = 0xF00013175 | ((scldel & 0x0F) << 20) | ((sdadel & 0x0F) << 16);
	I2C_ITConfig(I2C1, I2C_IT_TXI | I2C_IT_TCI | I2C_IT_NACKI | I2C_IT_STOPI, DISABLE);
	I2C_DMACmd(I2C1, I2C_DMAReq_Rx, ENABLE);
	I2C_Cmd(I2C1, ENABLE);
}

void I2C_connectToSlave_Write(uint8_t HardSlaveAddr)
{
	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET) {};

	if(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) == RESET)
	{
		//I2C_delay();
		I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
	}
    //I2C1->CR2 &= ~(I2C_CR2_RD_WRN);
   // I2C1->CR2 &= ~(0xff << 16);
   // I2C1->CR2 |= I2C_CR2_START | (1 << 16);
	//while(I2C1->CR2 & I2C_CR2_START);
	//I2C1->TXDR = SLAVE_ADDR;
	//while (!(I2C1->ISR & I2C_ISR_TXE));
    //I2C1->TXDR = 8;
    //while (!(I2C1->ISR & I2C_ISR_TXE));
    //I2C1->CR2 |= I2C_CR2_STOP;
    //while(I2C1->CR2 & I2C_CR2_STOP);
	//I2C_SendData(I2C1, SLAVE_ADDR);
	//for(i = 0; i < 50; i++) {};
	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	//I2C_GenerateSTOP(I2C1, ENABLE);


}

void I2C_connectToSlave_Read(uint8_t HardSlaveAddr)
{
	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET) {};

	if(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) == RESET)
	{
		//I2C_delay();
		I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Read);
	}



}

void I2C1_EV_IRQHandler()
{


}

void I2C_interrupt_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void I2C_delay()
{
	for(int i = 0; i < 500; i++) {};
}

void I2C_writeByte(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddr, uint8_t Byte)
{
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET) {};

	I2C_TransferHandling(I2C1, HardSlaveAddr, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	I2C_delay();

	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};

	I2C_SendData(I2C1, ExtSlaveAddr);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	I2C_delay();
	GPIOE->ODR = GPIO_Pin_14;


	I2C_SendData(I2C1, Byte);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	I2C_delay();
	GPIOE->ODR = GPIO_Pin_15;

	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};

	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);


}

void I2C_writeByte12(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddr, uint8_t Byte, uint8_t number)
{

	if(number == 0)
	{
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET) {};

	I2C_TransferHandling(I2C1, HardSlaveAddr, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	I2C_delay();

	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};

	I2C_SendData(I2C1, ExtSlaveAddr);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	I2C_delay();
	GPIOE->ODR = GPIO_Pin_14;


	I2C_SendData(I2C1, Byte);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	I2C_delay();
	GPIOE->ODR = GPIO_Pin_15;

	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};

	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
	}

	if(number == 1)
	{
		while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET) {};

		I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

		while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
		I2C_delay();
		GPIOE->ODR = GPIO_Pin_14;

		I2C_SendData(I2C1, Byte);
		while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
		I2C_delay();
		GPIOE->ODR = GPIO_Pin_15;

		I2C_GenerateSTOP(I2C1, ENABLE);
		while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};

		I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
	}

}

void I2C_connectToSlaveToWrite(uint8_t HardSlaveAddr)
{
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET) {};

	I2C_TransferHandling(I2C1, HardSlaveAddr, 3, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
}

void I2C_connectToSlaveToRead(uint8_t HardSlaveAddr)
{
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET) {};

	I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Read);

}

void I2C_readByte1()
{
	I2C_delay();
	return I2C_ReceiveData(I2C1);
}

void I2C_writeByte1(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddr, uint8_t Byte)
{
	uint16_t delay = 50000;
	uint16_t i = 0;

	I2C_delay();

	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};

	//I2C_SendData(I2C1, ExtSlaveAddr);
	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	//for(i = 0; i < delay; i++) {};
	//I2C_GenerateSTOP(I2C1, ENABLE);
	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};

	//I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

	GPIOE->ODR = GPIO_Pin_14;

	//for(i = 0; i < delay; i++) {};
	//I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	//for(i = 0; i < delay; i++) {};
	//if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	//{
	//	GPIOE->ODR = GPIO_Pin_13;
	//}
	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	I2C_SendData(I2C1, Byte);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};
	I2C_delay();

}

void I2C_generateStop()
{
	GPIOE->ODR = GPIO_Pin_15;
	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
	I2C_delay();
}



uint16_t I2C_readByte(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddress)
{
	uint16_t delay = 50000;
	uint16_t i = 0;

	uint16_t data = 0;
	I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	I2C_delay();
	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}

	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};

	I2C_SendData(I2C1, ExtSlaveAddress);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};


	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);


	I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Read);
	I2C_delay();
	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET) {};

	data = I2C_ReceiveData(I2C1);
	I2C_delay();

	//data |= I2C_ReceiveData(I2C1) << 8;

	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

	return data;
}



uint16_t I2C_readByte2(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddress)
{
	uint16_t delay = 50000;
	uint16_t i = 0;

	uint16_t data = 0;
//	I2C_TransferHandling(I2C1, 0, 1, I2C_SoftEnd_Mode, 0);

	I2C_GenerateSTART(I2C1, ENABLE);

	I2C_delay();

	I2C_SendData(I2C1, 0b00111100);
	I2C_delay();

	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}

	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};

	I2C_SendData(I2C1, ExtSlaveAddress);
	I2C_delay();
	//while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};


	I2C_GenerateSTOP(I2C1, ENABLE);
	I2C_delay();
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);


	//I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Read);
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_delay();
	I2C_SendData(I2C1, 0b00111101);
	I2C_delay();

	data = I2C_ReceiveData(I2C1);
	I2C_delay();

	//data |= I2C_ReceiveData(I2C1) << 8;

	I2C_GenerateSTOP(I2C1, ENABLE);
	I2C_delay();
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

	return data;
}

uint16_t I2C_readByte12(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddress, uint8_t	 number)
{
	uint16_t delay = 50000;
	uint16_t i = 0;

	uint16_t data = 0;
	if(number == 0) {
	I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	I2C_delay();
	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}

	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};

	I2C_SendData(I2C1, ExtSlaveAddress);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXE) == RESET) {};


	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);


	I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Read);
	I2C_delay();
	if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
	{
		GPIOE->ODR = GPIO_Pin_13;
	}
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET) {};

	data = I2C_ReceiveData(I2C1);
	I2C_delay();

	//data |= I2C_ReceiveData(I2C1) << 8;

	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
	}

	if(number == 1)
	{
		I2C_TransferHandling(I2C1, HardSlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Read);
		I2C_delay();
		if(I2C_GetFlagStatus(I2C1, I2C_ISR_NACKF) == 0)
		{
			GPIOE->ODR = GPIO_Pin_13;
		}
		while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET) {};

		data = I2C_ReceiveData(I2C1);
		I2C_delay();

			//data |= I2C_ReceiveData(I2C1) << 8;

		I2C_GenerateSTOP(I2C1, ENABLE);
		while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET) {};
		I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
	}

	return data;
}
