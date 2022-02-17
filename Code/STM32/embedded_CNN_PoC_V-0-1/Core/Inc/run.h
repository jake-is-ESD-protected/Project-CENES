/*
 * Personal abstraction layer because cube's is confusing
 *
 *
 * */

#ifndef _RUN_H_
#define _RUN_H_

#include "main.h"
#include "filterbank.h"

CRC_HandleTypeDef hcrc;

DMA_HandleTypeDef handle_GPDMA1_Channel15;

I2C_HandleTypeDef hi2c1;

SAI_HandleTypeDef hsai_BlockA1;

UART_HandleTypeDef huart2;

int run(void);

#endif
