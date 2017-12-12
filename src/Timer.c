#include "Timer.h"


void InitTimer2()
{

	TIM_TimeBaseInitTypeDef timer_struct;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	timer_struct.TIM_ClockDivision = 1;
	timer_struct.TIM_CounterMode = TIM_CounterMode_Up;
	timer_struct.TIM_Prescaler = 32;   // precision
	timer_struct.TIM_Period =  50000;	// tiempo total

	TIM_TimeBaseInit(TIM2,&timer_struct);

	/* TIM4 configuration: Input Capture mode Channel1 */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);



//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =	0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd =	ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

}



