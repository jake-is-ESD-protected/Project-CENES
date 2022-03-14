/*
*****************************************************************************************
file name:       	entry.cpp
auth:               Jakob Tschavoll
date:               02.03.22
brief:              step-in function, main entry. Init all entities.
version:            V0.4
notes:
*****************************************************************************************
*/

#include <entry.h>
#include "uterm.h"
#include "core.h"


int entry(void){


	// init entity core
	core_fsm.init();

	// init entity uterm
	uTerm.reset();
	uTerm.print_banner();
	uTerm.printf(UTERM);

//	char snd[50] = {0};
//	uint8_t len = sprintf(snd, "bauds=%d", 921600);
//	for(uint8_t i = 0; i < 3; i++)
//	{
//		snd[len] = 0xFF;
//		len++;
//	}
//	HAL_UART_Transmit(&huart2, (uint8_t*)snd, len, -1);



	// init entity mailbox
	uart_mbox.init();

	// init entity filterbank
	fbank.init();

	// init entity buffer
	buf.init();

	// init entity gpio
	gpio_D7.init(GPIOF, GPIO_PIN_13);
	gpio_D9.init(GPIOD, GPIO_PIN_15);


	return 0;
}




