#include "uterm.h"
#include "core.h"

// singleton instance
uTerminal uTerm(&hlpuart1);



void uTerminal::reset(void)
{
	HAL_UART_DeInit(pUart);
	HAL_UART_Init(pUart);
	HAL_StatusTypeDef stat = HAL_UART_Receive_IT(pUart, (uint8_t*)rx_buf, UTERM_RX_BUF_SIZE/2);
	if(stat != HAL_OK)
	{
		// TODO: err-handler
	}
}



void uTerminal::handle(cmd c)
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

	printf(UTERM);

	// This should not be needed
	reset();
}



HAL_StatusTypeDef uTerminal::printf(const char *fmt, ...)
{
	if(is_avail())
	{
		lock();

		memset(tx_buf, 0, UTERM_TX_BUF_SIZE);
		va_list args;
		va_start(args, fmt);
		vsnprintf(tx_buf, sizeof(tx_buf), fmt, args);
		va_end(args);
		int len = strlen(tx_buf);
		HAL_StatusTypeDef stat = HAL_UART_Transmit(pUart, (uint8_t*)tx_buf, len, -1);

		if(stat != HAL_OK)
		{
			// TODO: err-handler
		}

		unlock();
		return stat;
	}
	else
	{
		return HAL_BUSY;
	}
}



void uTerminal::print_banner(void)
{
	printf(N4CED_BANNER_S1);
	printf(N4CED_BANNER_S2);
	printf(N4CED_BANNER_S3);
	printf(N4CED_BANNER_S4);
	printf(N4CED_BANNER_S5);
	printf(N4CED_BANNER_S6);
}



void uTerminal::rx_callback()
{
	cmd c = str2cmd();
	uart_mbox.push(c, MBOX_FROM_ISR, core_fsm.hUI_task, INC_MSG_FLAG);
}



void uTerminal::__DEBUG_tx(float dbfs)
{
	uint16_t len = sprintf(tx_buf, "%.0f ", dbfs);
	HAL_UART_Transmit(pUart, (uint8_t*)tx_buf, len, -1);
}
