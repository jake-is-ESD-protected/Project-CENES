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

// RX/TX buffer size
#define UART_TX_BUF_SIZE	256
#define UART_RX_BUF_SIZE	8

#define LITTLE_BASH			"N4CED-Little-BashV1:~$ "


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



	/* abstraction for `HAL_UART_RxCpltCallback`
	 * @params: 	`stat`:	HAL-status which is not `HAL_OK`
	 * @returns 	`void`
	 * @brief:		error handler for uart
	 * @notes:
	 * */
	void handle_error(HAL_StatusTypeDef stat);



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

};

// singleton reference
extern uart srl_uart;


#endif
