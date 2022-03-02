/*
*****************************************************************************************
file name:       	serial.h
auth:               Jakob Tschavoll
date:               25.02.22
brief:              UART-peripheral abstraction as object
version:            V0.1
notes:				ATM, only commands of 4 characters can be sent: more get cut off, less cause bugs
*****************************************************************************************
*/


#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "main.h"
#include "run.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "lineart.h"
#include "literals.h"
#include "mailbox.h"


// Command translations via little bash
#define STR_CMD_INFO	"info\r\n\0\0"
#define STR_CMD_EEGG	"eegg\r\n\0\0"


// RX/TX buffer size
#define UART_TX_BUF_SIZE	256
#define UART_RX_BUF_SIZE	8


/* [CLASS] uart
 * @brief:	objectification of the UART peripheral
 * @intent:	data exchange over UART-based control "little-bash"
 * */
class uart
{
public:

	/* UART-constructor
	 * @params: 	`huart`: handle to UART-peripheral in HAL
	 * @returns 	`uart`-object
	 * @brief:		`Send and receive serial data`
	 * @notes:		implemented as singleton
	 * */
	uart(UART_HandleTypeDef* huart);



	/* Initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		`init UART IT RX`
	 * @notes:		needed because singleton-implementation forces
	 * 				a method which inits the UART IT settings AFTER
	 * 				HAL_init(), since the uart-object is created before
	 * 				the call of main(), similar to arduino `Serial.begin()`
	 * */
	void reset(void);



	/* print title card of device
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		print stylized title card to uart as a greeting
	 * @notes:
	 * */
	void print_banner(void);



	/* availability check
	 * @params: 	`void`
	 * @returns 	`true/false`
	 * @brief:		Check if underlying hardware is currently in use
	 * @notes:
	 * */
	bool is_avail(void);



	/* serial-printf
	 * @params: 	`string` (any)
	 * @returns 	`HAL_Status`
	 * @brief:		print data to serial (tx)
	 * @notes:		use like arduino `Serial.write()`
	 * */
	HAL_StatusTypeDef printf(const char *fmt, ...);



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



private:
	UART_HandleTypeDef* pUart;

	bool locked = false;

	char tx_buf[UART_TX_BUF_SIZE];
	char rx_buf[UART_RX_BUF_SIZE] = {0, 0, 0, 0, '\r', '\n', '\0', 0};
	char* rx_idx = &rx_buf[0];



	/* mutex-like
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		lock the uart peripheral
	 * @notes:
	 * */
	void lock(void){locked = true;}



	/* mutex-like
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		unlock the uart peripheral
	 * @notes:
	 * */
	void unlock(void){locked = false;}


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
			cmd c = {.type = get_help, .origin = little_bash_e, .destination = little_bash_e, .prio = LOW, .unused = 0};
			return c;
		}
		else if(!strcmp(rx_buf, STR_CMD_EEGG))
		{
			// print easteregg
			cmd c = {.type = easteregg, .origin = little_bash_e, .destination = little_bash_e, .prio = LOW, .unused = 0};
			return c;
		}
		else
		{
			// print "unknown"-message
			cmd c = {.type = no_info, .origin = little_bash_e, .destination = little_bash_e, .prio = LOW, .unused = 0};
			return c;
		}

	}
};

// singleton reference
extern uart srl_uart;


#endif
