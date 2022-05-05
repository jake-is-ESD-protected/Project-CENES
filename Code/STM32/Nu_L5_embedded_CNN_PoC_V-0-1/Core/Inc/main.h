#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l5xx_hal.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_ICACHE_Init(void);
void MX_DMA_Init(void);
void MX_SAI1_Init(void);
void MX_USART2_UART_Init(void);
void MX_CRC_Init(void);
void MX_LPUART1_UART_Init(void);
void MX_SDMMC1_SD_Init(void);
void StartDefaultTask(void *argument);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
