#include "RAI.h"
#include "USART_Lib.h"
#include "main.h"


void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	counter++;
}










