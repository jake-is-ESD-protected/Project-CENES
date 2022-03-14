#include "nextion.h"

nextionHMI nextion(&huart2);


void nextionHMI::reset(void)
{
	HAL_UART_DeInit(pUart);
	HAL_UART_Init(pUart);
	HAL_StatusTypeDef stat = HAL_UART_Receive_IT(pUart, (uint8_t*)rx_buf, NEX_RX_BUF_SIZE/2);
	if(stat != HAL_OK)
	{
		// TODO: err-handler
	}
}


void nextionHMI::tx(char* id, char* str)
{
	uint8_t len = sprintf(tx_buf, "%s.txt=\"%s\"", id, str);
	HAL_UART_Transmit_DMA(pUart, (uint8_t*)tx_buf, len);
	HAL_UART_Transmit_DMA(pUart, (uint8_t*)nex_end, 3);
}

void nextionHMI::tx(char* id, int16_t val)
{
	uint8_t len = sprintf(tx_buf, "%s.val=%d", id, val);
	for(uint8_t i = 0; i < 3; i++)
	{
		tx_buf[len] = 0xFF;
		len++;
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)tx_buf, len, -1);
}



void nextionHMI::rx_callback(void)
{
	uint8_t page = rx_buf[1];
	uint8_t id = rx_buf[2];
	uint8_t action = rx_buf[3];




}
