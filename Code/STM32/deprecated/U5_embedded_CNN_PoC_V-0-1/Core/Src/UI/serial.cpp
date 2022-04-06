#include <serial.h>

// singleton instance
uart srl_uart(&huart1);



uart::uart(UART_HandleTypeDef* huart)
{
	// tx init done in main.c (codegen)
	pUart = huart;
}



void uart::reset(void)
{
	HAL_UART_DeInit(pUart);
	HAL_UART_Init(pUart);
	HAL_StatusTypeDef stat = HAL_UART_Receive_IT(pUart, (uint8_t*)rx_buf, UART_RX_BUF_SIZE/2);
	if(stat != HAL_OK)
	{
		handle_error(stat);
	}
}



bool uart::is_avail(void)
{
	return !locked;
}



HAL_StatusTypeDef uart::printf(const char *fmt, ...)
{
	if(is_avail())
	{
		lock();

		memset(tx_buf, 0, UART_TX_BUF_SIZE);
		va_list args;
		va_start(args, fmt);
		vsnprintf(tx_buf, sizeof(tx_buf), fmt, args);
		va_end(args);
		int len = strlen(tx_buf);
		HAL_StatusTypeDef stat = HAL_UART_Transmit(pUart, (uint8_t*)tx_buf, len, -1);

		if(stat != HAL_OK)
		{
			handle_error(stat);
		}

		unlock();
		return stat;
	}
	else
	{
		return HAL_BUSY;
	}
}



void uart::rx_callback()
{
	// enter the core here
	printf("[ECHO]: ");
	printf(rx_buf);

	printf(LITTLE_BASH);

	// This should not be needed
	reset();

}



void uart::handle_error(HAL_StatusTypeDef stat)
{
	// TODO
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	srl_uart.rx_callback();
}
