
#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <inttypes.h>


/**
 * @defgroup  SYSTICK SYSTICK
 * @brief     SYSTICK control functions.
 */

/**
 * @addtogroup SYSTICK
 * @{
 */
void      SYSTICK_Init    (uint32_t freq);
uint32_t  SYSTICK_GetTime (void);



#endif /* SYSTICK_H_ */
