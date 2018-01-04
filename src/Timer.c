#include "Timer.h"

void Systick_Configuration(void)
{

	RCC_GetClocksFreq(&RCC_Clocks);
}

void Delay(uint32_t nTime)
{
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void Delay_lcd(uint32_t nTime)
{
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100000);	//Trabajamos en microsegundos
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{

	if (TimingDelay != 0)
	{
		TimingDelay--;
	};
}

void SysTick_Handler(void) {

	TimingDelay_Decrement();

}

