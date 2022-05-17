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
#include "ICS43432_correction.h"


int entry(void){

//	FRESULT res; 									/* FatFs function common result code */
//	uint32_t byteswritten; 							/* File write/read counts */
//	uint8_t wtext[] = "STM32 FATFS works great!"; 	/* File write buffer */
//	uint8_t rtext[_MAX_SS];							/* File read buffer */
//
//	res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
//	if(res != FR_OK)
//	{
//		Error_Handler();
//	}
//	else
//	{
//		//Open file for writing (Create)
//		res = f_open(&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE);
//		if(res != FR_OK)
//		{
//			Error_Handler();
//		}
//		else
//		{
//			//Write to the text file
//			res = f_write(&SDFile, wtext, strlen((char *)wtext), (UINT*)&byteswritten);
//			if((byteswritten == 0) || (res != FR_OK))
//			{
//				Error_Handler();
//			}
//			else
//			{
//				f_close(&SDFile);
//			}
//		}
//	}
//	f_mount(&SDFatFS, (TCHAR const*)NULL, 0);


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
	a_weight.init(A_coeffs);

#ifdef ICS43432_CORRECTION
	ICS43432_correction_filter.init(ICS43432_correction_coeffs);
#endif

	// init entity gpio
	gpio_D2.init(GPIOF, GPIO_PIN_15);
	gpio_D3.init(GPIOE, GPIO_PIN_13);
	gpio_D4.init(GPIOF, GPIO_PIN_14);

	// init cnn
	cnn_instance.init();

	// init entity buffer
	vTaskDelay(500 / portTICK_PERIOD_MS);
	buf.init();


	return 0;
}




