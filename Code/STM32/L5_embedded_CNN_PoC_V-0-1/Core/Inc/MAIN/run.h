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

#include "serial.h"
#include "main.h"


extern CRC_HandleTypeDef hcrc;

extern SAI_HandleTypeDef hsai_BlockA1;
extern DMA_HandleTypeDef hdma_sai1_a;

extern SPI_HandleTypeDef hspi1;

extern UART_HandleTypeDef huart1;


int run(void);

#endif
