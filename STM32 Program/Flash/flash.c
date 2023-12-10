#include "Flash/flash.h"
#include "stm32f30x.h"

void Flash_init()
{

}

void Flash_Write(uint8_t pageNumber, uint32_t data)
{
    //static uint8_t dataFromFlash;
    //dataFromFlash = FLASH_Read(pageNumber * FLASH_PAGE_ADDRESS);

	uint32_t address = pageNumber * 1024 + FLASH_PAGE_ADDRESS;

	FLASH_Unlock();
    FLASH_ErasePage(address);

    FLASH_ProgramWord(address, data);
    FLASH_Lock();
}

uint32_t FLASH_Read(uint8_t pageNumber)
{
	uint32_t address = pageNumber * 1024 + FLASH_PAGE_ADDRESS;
    return (*(__IO uint32_t*)address);
}
