/*
 * f_name:	mymain.c
 * auth:	Jakob Tschavoll
 * ver:		1.3
 * brief:	abstraction for IDE-specific main.c for cleaner code
 * */

#include "run.h"


// buffers
uint32_t rx_buf[FRAME_SIZE];
uint32_t tx_buf[FRAME_SIZE];
volatile static uint32_t* pIn = &rx_buf[0];
volatile static uint32_t* pOut = &tx_buf[FRAME_SIZE / 2];


// flags
uint8_t hlfcplt = 0;
uint8_t cplt = 0;


// functions
int run(void){

	int do_dsp = 1;
	init_filterbank();


	while(1){

		if(hlfcplt || cplt){
			if(do_dsp){

				dsp(pIn, pOut);
			}
			else{ // feedthrough for testing
				feedthrough(pIn, pOut);
			}
			hlfcplt = 0;
			cplt = 0;
		}
	}
}


// callbacks
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef* hsai){

	pIn = &rx_buf[0];
	pOut = &tx_buf[FRAME_SIZE];
	hlfcplt = 1;

}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef* hsai){

	pIn = &rx_buf[FRAME_SIZE / 2];
	pOut = &tx_buf[0];
	cplt = 1;
}
