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
#include "A_weighting.h"


int entry(void){


	// init entity core
	core_fsm.init();

	// init entity uterm
	uTerm.reset();
	uTerm.print_banner();
	uTerm.printf(UTERM);

	// init entity nextion-display
	nextion.reset();

	// init entity mailbox
	uart_mbox.init();

	// init entity filterbank
	fbank.init();
	A_filter.init(A_coeffs);

	// init entity buffer
	buf.init();

	// init entity gpio
	gpio_D7.init(GPIOF, GPIO_PIN_13);
	gpio_D9.init(GPIOD, GPIO_PIN_15);
	gpio_D6.init(GPIOE, GPIO_PIN_9);

	// init cnn
	cnn_instance.init();


	return 0;
}




