#include "stm32l1xx.h"

#ifndef RAI_H_
#define RAI_H_

void SysTick_Handler(void);
void I2C1_ER_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void USART2_IRQHandler(void);

#endif /* RAI_H_ */
