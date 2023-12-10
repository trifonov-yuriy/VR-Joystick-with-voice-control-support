#include "WWatchDog/wwatchdog.h"

void Init_WWatchDog()
{
	WWDG_TypeDef wwdg_TypeDef;

	RCC_APB1PeriphClockCmd(RCC_APB1ENR_WWDGEN, ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(WWATCH_DOG_TIMER);

    WWDG_Enable(WWATCH_DOG_TIMER);


}


