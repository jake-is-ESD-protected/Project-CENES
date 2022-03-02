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
#include "cmsis_os.h"
#include "err.h"


#define  	QUEUE_TIMEOUT		1
#define 	QUEUE_NO_TIMEOUT	osWaitForever
#define 	QUEUE_FROM_ISR		0

#define 	QUEUE_SIZE			10


typedef enum c_type
{
	no_info,
	sleep,
	get_help,
	switch_rta,
	switch_ai,
	easteregg
}cmd_t;



typedef struct {
	cmd_t type;
	entity_t origin;
	entity_t destination;
	prio_t prio;
	uint8_t unused;
}cmd;



class mbox
{
public:

	osMessageQueueId_t hQueue;


	/* mailbox-constructor
	 * @params: 	`void`
	 * @returns 	`mbox`-object
	 * @brief:		Init a queue wrapper
	 * @notes:		is now callable via hQueue
	 * */
	mbox();



	/* mailbox-initialiser
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		init the underlying queue
	 * @notes:		has to be initialized after scheduler
	 * */
	void init(void);



	/* mailbox insertion
	 * @params: 	`cmd`-struct
	 * @params:		`bool from_ISR`: specify if mbox is filled inside an ISR
	 * @returns 	`void`
	 * @brief:		push a command object to the mailbox's fifo buffer
	 * @notes:
	 * */
	void push(cmd cmd, bool from_ISR);



	/* mailbox retrieval
	 * @params: 	`bool from_ISR`: specify if mbox is emptied inside an ISR
	 * @returns 	`cmd`-object
	 * @brief:		acquire the oldest command in the buffer
	 * @notes:		**This function is blocking, only call it when data is available**
	 * */
	cmd pop(bool from_ISR);



	/* check for data in queue
	 * @params: 	`void`
	 * @returns 	`bool`: true if data is available
	 * @brief:		peek into queue and check for pending messages
	 * @notes:		use this as the entry for `pop()`
	 * */
	bool data_avail(void);



private:


};

// global references
extern mbox uart_mbox;

#endif
