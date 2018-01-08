/**
 * @file    fat.h
 * @brief   FAT file system implementation.
 * @date    4 maj 2014
 * @author  Michal Ksiezopolski
 *
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the
 * accompanying materials are made available
 * under the terms of the GNU Public License
 * v3.0 which accompanies this distribution,
 * and is available at
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#ifndef FAT_H_
#define FAT_H_

#include <inttypes.h>

#include "USART_Lib.h"

/**
 * @defgroup  FAT FAT
 * @brief     FAT filesystem functions.
 */

/**
 * @addtogroup FAT
 * @{
 */

int8_t FAT_Init(uint8_t (*phyInit)(void),
    uint8_t (*phyReadSectors)(uint8_t* buf, uint32_t sector, uint32_t count),
    uint8_t (*phyWriteSectors)(uint8_t* buf, uint32_t sector, uint32_t count));

int FAT_OpenFile(const char* filename);
int FAT_ReadFile(int file, uint8_t* data, int count);
int FAT_MoveRdPtr(int file, int newWrPtr);
int FAT_MoveWrPtr(int file, int newWrPtr);
int FAT_WriteFile(int file, const uint8_t* data, int count);

int FAT_RdPtr(int file);
int FAT_WrPtr(int file);


int FAT_SDWriteFloatFile(int file, float flot);
int FAT_SDJumpLineFile(int file);

/**
 * @}
 */

#endif /* FAT_H_ */

