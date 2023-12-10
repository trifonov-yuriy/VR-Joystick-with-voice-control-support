#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_misc.h>
#include <stm32f30x_spi.h>
#include <FreeRTOSConfig.h>
#include <projdefs.h>
#include <portmacro.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <croutine.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdbool.h>

#define SPI_GPIO		 	 GPIOA
#define SPI_GPIO_Pins	 	 GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define SPI_GPIO_CS		 	 GPIOE
#define SPI_GPIO_CS_Pin	     GPIO_Pin_3


#define SPI3_GPIO		 	 GPIOB
#define SPI3_GPIO_SCK       GPIO_Pin_3
#define SPI3_GPIO_MISO       GPIO_Pin_4
#define SPI3_GPIO_MOSI       GPIO_Pin_5

#define SPI3_GPIO_Pins	 	 SPI3_GPIO_SCK | SPI3_GPIO_MISO | SPI3_GPIO_MOSI
#define SPI3_GPIO_CS		 GPIOA
#define SPI3_GPIO_CS_Pin	 GPIO_Pin_4


void SPI_GPIO_INIT();


void SPI_INIT();

void SPI1_IRQHandler();

void SPI_NVIC_INIT();

void SPI_writeData(uint8_t address, uint8_t dataToWrite);

uint8_t SPI_sendByte(uint8_t byteToSend);


void SPI3_GPIO_init();

void SPI3_init();
