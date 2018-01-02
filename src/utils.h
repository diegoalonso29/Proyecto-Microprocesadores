#ifndef UTILS_H_
#define UTILS_H_

#include <inttypes.h>

/**
 * @defgroup  UTILS UTILS
 * @brief     Common utility functions and macros
 */

/**
 * @addtogroup UTILS
 * @{
 */

void hexdump(const uint8_t* buf, uint32_t length);
void hexdumpC(const uint8_t const *buf, uint32_t length);
void hexdump16C(const uint16_t const *buf, uint32_t length);
uint32_t ntohl(uint32_t val);
uint8_t isBigEndian(void);


/**
 * @}
 */


#endif /* UTILS_H_ */
