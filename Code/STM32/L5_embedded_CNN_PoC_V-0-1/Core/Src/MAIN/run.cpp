/*
 * f_name:	run.c
 * auth:	Jakob Tschavoll
 * ver:		1.3
 * brief:	abstraction for IDE-specific main.c for cleaner code
 * */

#include "serial.h"
#include "run.h"
#include "filterbank.h"
#include "sample.h"
#include "core.h"


int run(void){

	srl_uart.reset();
	srl_uart.printf(N4CED_BANNER_S1);
	srl_uart.printf(N4CED_BANNER_S2);
	srl_uart.printf(N4CED_BANNER_S3);
	srl_uart.printf(N4CED_BANNER_S4);
	srl_uart.printf(N4CED_BANNER_S5);
	srl_uart.printf(N4CED_BANNER_S6);
	srl_uart.printf(LITTLE_BASH);

	core_fsm.init();

	//init_filterbank();

//	init_buffers();
//	init_i2s_dma();
//	start_sampler();


	while(1){

		//sample();
	}
}




