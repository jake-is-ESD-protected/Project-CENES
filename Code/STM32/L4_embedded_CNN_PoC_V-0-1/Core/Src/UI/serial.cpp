#include <serial.h>
#include "core.h"

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
		err e = {.type = generic, .origin = little_bash_e, .prio = HIGH, .description = &stat};
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
			err e = {.type = generic, .origin = little_bash_e, .prio = HIGH, .description = &stat};
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
	uart_mbox.push(c, MBOX_FROM_ISR, core_fsm.hUI_task, INC_MSG_FLAG);
}



void uart::handle(cmd c)
{
	switch(c.type)
	{
	case no_info:
		printf(TXT_UNKNOWN);
		break;
	case get_help:
		printf(TXT_INFO);
		break;
	case easteregg:
		printf(EASTEREGG);
		break;
	default:
		printf(TXT_UNKNOWN);
	}

	printf(LITTLE_BASH);

	// This should not be needed
	reset();
}



void uart::print_rta(float* pData)
{
	uint16_t buf_pos = 0;
	char clear[] = "\033[2J";
	HAL_UART_Transmit(pUart, (uint8_t*)clear, sizeof(clear), -1);
	tx_buf[buf_pos] = '\r'; buf_pos++;
	tx_buf[buf_pos] = '\n'; buf_pos++;

	for(uint8_t i = 0; i < N_BANDS/2; i++) // only use every second band (save space)
	{
		int16_t lvl = ((int16_t)(pData[i*2])) / -10;
		for(uint8_t j = 0; j < lvl; j++)
		{
			tx_buf[buf_pos] = '|';
			buf_pos++;
		}
		tx_buf[buf_pos] = '\r'; buf_pos++;
		tx_buf[buf_pos] = '\n'; buf_pos++;
	}
	HAL_UART_Transmit(pUart, (uint8_t*)tx_buf, buf_pos, -1);
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	srl_uart.rx_callback();
}
