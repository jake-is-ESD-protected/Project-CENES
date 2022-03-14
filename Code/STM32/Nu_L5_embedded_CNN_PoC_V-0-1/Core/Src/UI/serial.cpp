#include <serial.h>
#include "core.h"


uart::uart(UART_HandleTypeDef* huart)
{
	// tx init done in main.c (codegen)
	pUart = huart;
}



bool uart::is_avail(void)
{
	return !locked;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if(huart == &hlpuart1)
	{
		uTerm.rx_callback();
	}
	if(huart == &huart2)
	{
		nextion.rx_callback();
	}
}


