
#ifndef TIMERS_H_
#define TIMERS_H_

#include <inttypes.h>

/**
 * @defgroup  TIMER TIMER
 * @brief     Timing control functions
 */

/**
 * @addtogroup TIMER
 * @{
 */

void      TIMER_Init              (uint32_t freq);
void      TIMER_DelayUS           (uint32_t us);
void      TIMER_Delay             (uint32_t ms);
uint8_t   TIMER_DelayTimer        (uint32_t ms, uint32_t startTime);
int8_t    TIMER_AddSoftTimer      (uint32_t maxVal, void (*fun)(void));
void      TIMER_StartSoftTimer    (uint8_t id);
void      TIMER_SoftTimersUpdate  (void);
uint32_t  TIMER_GetTime           (void);



#endif /* TIMERS_H_ */

