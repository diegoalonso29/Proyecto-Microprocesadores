#ifndef TIMERS_H_
#define TIMERS_H_

#include "stm32l1xx_nucleo.h"
#include "stm32l1xx.h"

static volatile uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);
void Delay_lcd(uint32_t nTime);
void SysTick_Handler(void);



#endif /* TIMERS_H_ */


