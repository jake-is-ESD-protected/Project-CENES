/*
*****************************************************************************************
file name:       	uterm.h
auth:               Jakob Tschavoll
date:               10.03.22
brief:              terminal abstraction as object
version:            V0.1
notes:				ATM, only commands of 4 characters can be sent: more get cut off, less cause bugs
*****************************************************************************************
*/

#ifndef _UTERM_H_
#define _UTERM_H_

#include "serial.h"
#include "lineart.h"
#include "literals.h"

// Command translations via uTerm
#define STR_CMD_INFO	"info\r\n\0\0"
#define STR_CMD_EEGG	"eegg\r\n\0\0"
#define __DEBUG_FORMAT		"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\r\n"

#define UTERM_TX_BUF_SIZE	512
#define UTERM_RX_BUF_SIZE	8


/* [CLASS] uTerminal
 * @brief:	I/O terminal on chip for basic commands via Putty
 * @intent:	data exchange over UART-based control "uTerm"
 * */
class uTerminal: public uart
{
public:
	using uart::uart;

	float debug_buf[28];



	/* Initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		`init uTerm IT RX`
	 * @notes:		needed because singleton-implementation forces
	 * 				a method which inits the UART IT settings AFTER
	 * 				HAL_init(), since the uart-object is created before
	 * 				the call of main(), similar to arduino `Serial.begin()`
	 * */
	void reset(void);



	/* serial-printf
	 * @params: 	`string` (any)
	 * @returns 	`HAL_Status`
	 * @brief:		print data to serial (tx)
	 * @notes:		use like arduino `Serial.write()`
	 * */
	HAL_StatusTypeDef printf(const char *fmt, ...);



	/* print title card of device
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		print stylized title card to uart as a greeting
	 * @notes:
	 * */
	void print_banner(void);



	/* abstraction for `HAL_UART_RxCpltCallback`
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		callback for when UART receives messages
	 * @notes:
	 * */
	void rx_callback(void);



	/* convert commands into procedures & responses
	 * @params: 	`cmd c`: command to interpret
	 * @returns 	`void`
	 * @brief:		convert commands into prints & other responses by UART
	 * @notes:
	 * */
	void handle(cmd c);



	/*
	 * @params:
	 * @returns
	 * @brief:
	 * @notes:		TODO
	 * */
	void __DEBUG_tx(float dbfs);


private:

	char tx_buf[UTERM_TX_BUF_SIZE];
	char rx_buf[UTERM_RX_BUF_SIZE] = {0, 0, 0, 0, '\r', '\n', '\0', 0};

	/* convert RX-strings into known commands
	 * @params: 	`void`
	 * @returns 	`cmd`-struct
	 * @brief:		obtain commands from serial
	 * @notes:
	 * */
	cmd str2cmd(void)
	{
		if(!strcmp(rx_buf, STR_CMD_INFO))
		{
			// print info about system to terminal
			cmd c = {.type = get_help, .origin = uTerm_e, .destination = uTerm_e, .prio = LOW};
			return c;
		}
		else if(!strcmp(rx_buf, STR_CMD_EEGG))
		{
			// print easteregg
			cmd c = {.type = easteregg, .origin = uTerm_e, .destination = uTerm_e, .prio = LOW};
			return c;
		}
		else
		{
			// print "unknown"-message
			cmd c = {.type = no_info, .origin = uTerm_e, .destination = uTerm_e, .prio = LOW};
			return c;
		}
	}
};

// singleton reference
extern uTerminal uTerm;

#endif
