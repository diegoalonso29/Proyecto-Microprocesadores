#ifndef MAIN_H_
#define MAIN_H_

#include "MPU6050.h"
#include "USART_Lib.h"

#include <math.h>
#include <stdio.h>

void Systick_Configuration(void);
void TimingDelay_Decrement(void);
void Delay(uint32_t nTime);

void ftoa(float n, uint8_t *res, int afterpoint);
int intToStr(int x, uint8_t str[], int d);
void reverse(uint8_t *str, int len);

static volatile uint32_t TimingDelay;

#endif /* MAIN_H_ */
