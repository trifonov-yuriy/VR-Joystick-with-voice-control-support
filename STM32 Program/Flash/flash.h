#include <stm32f30x_flash.h>

#define FLASH_PAGE_ADDRESS   0x08000000

uint32_t FLASH_Read(uint8_t pageNumber);

void Flash_Write(uint8_t pageNumber, uint32_t data);
