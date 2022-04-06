/*
*****************************************************************************************
file name:       	run.h
auth:               Jakob Tschavoll
date:               25.02.22
brief:              Header for program entry
version:            V0.1
notes:				Makes handles available to underlying code
*****************************************************************************************
*/
#ifndef _RUN_H_
#define _RUN_H_

#include <serial.h>
#include "main.h"


extern CRC_HandleTypeDef hcrc;

extern I2C_HandleTypeDef hi2c1;

extern SAI_HandleTypeDef hsai_BlockA1;
extern DMA_NodeTypeDef Node_GPDMA1_Channel15;
extern DMA_QListTypeDef List_GPDMA1_Channel15;
extern DMA_HandleTypeDef handle_GPDMA1_Channel15;

extern UART_HandleTypeDef huart1;


int run(void);

#endif
