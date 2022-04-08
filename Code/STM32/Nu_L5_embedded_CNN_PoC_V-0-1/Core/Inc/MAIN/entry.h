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

#include "main.h"

//#define FGEN_SETUP

extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_lpuart1_tx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern SAI_HandleTypeDef hsai_BlockA1;
extern DMA_HandleTypeDef hdma_sai1_a;

int entry(void);

#endif
