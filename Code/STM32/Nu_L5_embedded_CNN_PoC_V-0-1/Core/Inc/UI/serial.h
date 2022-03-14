/*
*****************************************************************************************
file name:       	serial.h
auth:               Jakob Tschavoll
date:               25.02.22
brief:              UART-peripheral abstraction as object
version:            V0.1
notes:
*****************************************************************************************
*/


#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <entry.h>
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "mailbox.h"


#define INC_MSG_FLAG		0x00000010



/* [CLASS] uart
 * @brief:	objectification of the UART peripheral
 * @intent:
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



	/* availability check
	 * @params: 	`void`
	 * @returns 	`true/false`
	 * @brief:		Check if underlying hardware is currently in use
	 * @notes:
	 * */
	bool is_avail(void);



protected:
	UART_HandleTypeDef* pUart;


	bool locked = false;

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


#endif
