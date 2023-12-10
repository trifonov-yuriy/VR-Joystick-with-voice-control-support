#include <stm32f30x.h>
#include <stm32f30x_dma.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_i2c.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_tim.h>
#include <stdbool.h>
#include <FreeRTOSConfig.h>
#include <projdefs.h>
#include <portmacro.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <croutine.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include "LSM303DLHC/lsm303dlhc.h"

#define I2C1_PORT	 GPIOB
#define I2C1_SCL	 GPIO_Pin_6
#define I2C1_SDA	 GPIO_Pin_7

#define I2C_MASTER_ADDR  0x08
#define I2C_SLAVE_ADDR   0b0011001


volatile uint8_t I2C_adres;
volatile uint8_t I2C_data;
volatile uint8_t I2C_numberLed;

volatile uint8_t I2C_flag_adres;
volatile uint8_t I2C_flag_data;

volatile bool I2C_startTransmitFlag;
volatile bool I2C_startReceiveFlag;
volatile bool I2C_sendDataFlag;
volatile bool I2C_stopFlag;

volatile I2C_numberAxie;

void I2C_init();
void I2C_interrupt_Config();
void I2C_writeByte(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddress, uint8_t Byte);
uint16_t I2C_readByte(uint8_t HardSlaveAddr, uint8_t ExtSlaveAddress);
void I2C1_EV_IRQHandler();
void I2C1_connectToSlave(uint8_t HardSlaveAddr);
void I2C_initParameters();
void I2C_resetParameters();

