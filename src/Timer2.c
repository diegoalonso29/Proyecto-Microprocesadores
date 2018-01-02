
#include "stm32l1xx_nucleo.h"
#include "timer2.h"

/**
 * @addtogroup TIMER2
 * @{
 */

static volatile uint32_t usCount; ///< Microsecond counter

/**
 * @brief Initialize timer2 as microsecond counter
 */
void TIMER2_Init(void) {

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 15; // overflow every microsecond
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  // initialize interrupt
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM2, ENABLE); // enable timer
}
/**
 * @brief Get time value
 * @return Time in microseconds
 */
uint32_t TIMER2_GetTime(void) {

  return usCount;

}
/**
 * @brief IRQ handler for TIM2
 */
void TIM2_IRQHandler(void) {

  if((TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != RESET)) {
    // clear flag
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    // update microsecond count
    usCount++;
  }
}





