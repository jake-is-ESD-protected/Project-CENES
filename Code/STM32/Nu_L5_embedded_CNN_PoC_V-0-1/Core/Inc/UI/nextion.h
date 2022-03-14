/*
*****************************************************************************************
file name:       	nextion.h
auth:               Jakob Tschavoll
date:               10.03.22
brief:              display abstraction as object
version:            V0.1
notes:				Shape of RX-signal:
						start	|	page	|	id	|	action	|	end
						0x65	|	0xXX	|	0xXX|	0xXX	|	0xFF 0xFF 0xFF
*****************************************************************************************
*/

#ifndef _NEXTION_H_
#define _NEXTION_H_

#include "serial.h"
#include "params.h"


#define NEX_TX_BUF_SIZE	512
#define NEX_RX_BUF_SIZE	7

#define MAX_SPL_LVL_SHOWN	120.
#define MIN_SPL_LVL_SHOWN	-20.

#define BIN_REFRESH_STEP	(FPS / 6)
#define LVL_REFRESH_STEP	FPS
#define LEQ_5s				(FPS * 5)

#define NEX_BUF_RDY_FLAG	0x00000100U
#define NEX_LVL_RDY_FLAG	0x00001000U


/* [CLASS] nextionHMI
 * @brief:	objectification of the nextion display
 * @intent:	data exchange over on-device TFT-display
 * */
class nextionHMI: public uart
{
public:
	using uart::uart;

	// bank output formatted as value from 0% to 100%
	uint8_t disp_data[N_BANDS];
	int16_t disp_lvl;



	/* Initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		`init nextion IT RX`
	 * @notes:		needed because singleton-implementation forces
	 * 				a method which inits the UART IT settings AFTER
	 * 				HAL_init(), since the uart-object is created before
	 * 				the call of main(), similar to arduino `Serial.begin()`
	 * */
	void reset(void);



	void tx(char* id, char* str);



	void tx(char* id, int16_t val);



	void rx_callback(void);

//private:

	char tx_buf[NEX_TX_BUF_SIZE];
	char rx_buf[NEX_RX_BUF_SIZE];

	char bin_id_buf[N_BANDS][4] = {	"j38", "j37", "j36", "j35", "j34", "j33", "j32", "j31", "j30", "j29", "j28", "j27", "j26",
									"j25", "j24", "j23", "j22", "j21", "j20", "j19", "j18", "j17", "j16", "j15", "j14", "j13",
									"j12", "j11"};
	const char nex_end[3] = {0xFF, 0xFF, 0xFF};
};

extern nextionHMI nextion;

#endif
