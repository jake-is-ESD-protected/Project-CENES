/*
 * f_name:	run.c
 * auth:	Jakob Tschavoll
 * ver:		1.3
 * brief:	abstraction for IDE-specific main.c for cleaner code
 * */

#include <serial.h>
#include "run.h"
#include "filterbank.h"
#include "sample.h"


int run(void){

	srl_uart.reset();
	srl_uart.printf(N4CED_BANNER_S1);
	srl_uart.printf(N4CED_BANNER_S2);
	srl_uart.printf(N4CED_BANNER_S3);
	srl_uart.printf(N4CED_BANNER_S4);
	srl_uart.printf(N4CED_BANNER_S5);
	srl_uart.printf(N4CED_BANNER_S6);
	srl_uart.printf(LITTLE_BASH);



	//init_filterbank();

//	uint8_t nok = ssd1306_Init(&hi2c1);
//	if(nok){
//		uart_printf("OLED-driver problem occurred: %d\r\n", nok);
//	}

//	ssd1306_WriteString("init", Font_7x10, White);
//	ssd1306_UpdateScreen(&hi2c1);


	init_buffers();
//	init_i2s_dma();
//	start_sampler();


	while(1){

		//sample();
	}
}




