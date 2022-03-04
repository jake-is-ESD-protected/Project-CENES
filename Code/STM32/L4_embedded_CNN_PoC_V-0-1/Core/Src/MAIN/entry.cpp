/*
*****************************************************************************************
file name:       	entry.cpp
auth:               Jakob Tschavoll
date:               02.03.22
brief:              step-in function, main entry. Init all entities.
version:            V0.4
notes:				Thread currently unused after init
*****************************************************************************************
*/

#include <entry.h>
#include "serial.h"
#include "core.h"


int entry(void){


	// init entity core
	core_fsm.init();

	// init entity little_bash
	srl_uart.reset();
	srl_uart.print_banner();
	srl_uart.printf(LITTLE_BASH);

	// init entity mailbox
	uart_mbox.init();

	// init entity filterbank
	fbank.init();

	// init entity buffer
	buf.init();

	// init entity gpio
	gpio_D7.init(GPIOA, GPIO_PIN_8);
	gpio_D9.init(GPIOC, GPIO_PIN_7);



	return 0;
}




