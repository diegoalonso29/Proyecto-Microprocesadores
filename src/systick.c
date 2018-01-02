
#include "systick.h"
#include "stm32l1xx_nucleo.h"

/**
 * @defgroup  SYSTICK SYSTICK
 * @brief     SYSTICK control functions.
 */

/**
 * @addtogroup SYSTICK
 * @{
 */

static volatile uint32_t sysTicks;  ///< Delay timer.

/**
 * @brief Initialize the SysTick with a given frequency
 * @param freq SysTick frequency
 */
void SYSTICK_Init(uint32_t freq) {

  RCC_ClocksTypeDef RCC_Clocks;

  RCC_GetClocksFreq(&RCC_Clocks); // Complete the clocks structure with current clock settings.

  SysTick_Config(RCC_Clocks.HCLK_Frequency / freq); // Set SysTick frequency

}
/**
 * @brief Get the system time
 * @return System time.
 */
uint32_t SYSTICK_GetTime(void) {
  return sysTicks;
}
static volatile uint32_t counter;

/**
 * @brief Interrupt handler for SysTick.
 */
//void SysTick_Handler(void) {
//
//	TimingDelay_Decrement();
//	counter++;
//	sysTicks++; // Update system time
//
//}




