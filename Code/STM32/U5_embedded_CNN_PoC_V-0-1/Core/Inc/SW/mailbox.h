/*
*****************************************************************************************
file name:       	mailbox.h
auth:               Jakob Tschavoll
date:               25.02.22
brief:              Abstraction for message queues
version:            V0.1
notes:
*****************************************************************************************
*/

#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include "main.h"

class cmd
{
public:
	typedef enum type
	{
		turn_on,
		turn_off,
		get_help,
		switch_rta,
		switch_ai,
		unknwon
	}cmd_t;
	cmd_t type;

	typedef enum origin
	{
		MAIN,
		RT,
		SW,
		UI
	}origin_t;
	origin_t origin;

	typedef enum prio
	{
		LOW,
		HIGH
	}prio_t;
	prio_t prio;

};

class mbox
{
public:


	/* mailbox-constructor
	 * @params: 	`void`
	 * @returns 	`mbox`-object
	 * @brief:		Init a queue wrapper
	 * @notes:
	 * */
	mbox(void);



	/* mailbox insertion
	 * @params: 	`cmd`-object
	 * @returns 	`void`
	 * @brief:		push a command object to the mailbox's fifo buffer
	 * @notes:
	 * */
	void push(cmd cmd);



	/* mailbox retrieval
	 * @params: 	`void`
	 * @returns 	`cmd`-object
	 * @brief:		acquire the oldest command in the buffer
	 * @notes:		popping a command consumes it
	 * */
	cmd pop(void);



	/* mailbox retrieval
	 * @params: 	`void`
	 * @returns 	`bool` availability of data
	 * @brief:		peek into mailbox for data to read
	 * @notes:
	 * */
	bool data_avail(void);

};

#endif
