#ifndef MAIN_H_
#define MAIN_H_

#include "MPU6050.h"
#include "USART_Lib.h"
#include "Timer.h"
#include <math.h>
#include <stdio.h>

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);

static volatile uint32_t counter;
static volatile uint32_t TimingDelay;

#endif /* MAIN_H_ */
