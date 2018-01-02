
#ifndef SDCARD_H_
#define SDCARD_H_

#include "stm32l1xx_nucleo.h"

/**
  @defgroup  SD_CARD SD CARD
  @brief     SD Card library functions
 */

/**
  @addtogroup SD_CARD
  @{
 */

void    SD_Init         (void);
uint8_t SD_ReadBlock    (uint32_t block, uint8_t* buf);
uint8_t SD_ReadSectors  (uint8_t* buf, uint32_t sector, uint32_t count);
uint8_t SD_WriteSectors (uint8_t* buf, uint32_t sector, uint32_t count);
uint64_t SD_ReadCapacity(void);



#endif /* SDCARD_H_ */
