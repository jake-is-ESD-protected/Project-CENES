#include <serial.h>

// singleton instance
uart srl_uart(&huart2);



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
		err e = {.type = generic, .origin = little_bash_e, .prio = HIGH};
		e.handle();
	}
}



void uart::print_banner(void)
{
	printf(N4CED_BANNER_S1);
	printf(N4CED_BANNER_S2);
	printf(N4CED_BANNER_S3);
	printf(N4CED_BANNER_S4);
	printf(N4CED_BANNER_S5);
	printf(N4CED_BANNER_S6);
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
			err e = {.type = generic, .origin = little_bash_e, .prio = HIGH};
			e.handle();
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
	cmd c = str2cmd();
	uart_mbox.push(c, true);
}



void uart::handle(cmd c)
{
	if(c.type == no_info)
	{
		 printf(TXT_UNKNOWN);
	}

	else if(c.type == get_help)
	{
		printf(TXT_INFO);
	}
	else
	{
		printf(TXT_UNKNOWN);
	}

	printf(LITTLE_BASH);

	// This should not be needed
	reset();
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	srl_uart.rx_callback();
}
