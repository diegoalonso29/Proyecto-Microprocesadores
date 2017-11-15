/*
 * SPI_lib.h
 *
 *  Created on: Nov 15, 2017
 *      Author: Liberto
 */

#include "stm32l1xx_nucleo.h"

#ifndef SPI_H_
#define SPI_H_

#include <inttypes.h>

/**
 * @defgroup  SPI1 SPI1
 * @brief     SPI1 control functions
 */

/**
 * @addtogroup SPI1
 * @{
 */

uint8_t SPI1_Transmit       (uint8_t data);
void    SPI1_Init           (void);
void    SPI1_Select         (void);
void    SPI1_Deselect       (void);
void    SPI1_ReadBuffer     (uint8_t* buf, uint32_t len);
void    SPI1_WriteBuffer    (uint8_t* buf, uint32_t len);
void    SPI1_SendBuffer     (uint8_t* buf, uint32_t len);
void    SPI1_TransmitBuffer (uint8_t* rx_buf, uint8_t* tx_buf, uint32_t len);

/**
 * @}
 */

#endif /* SPI_H_ */
