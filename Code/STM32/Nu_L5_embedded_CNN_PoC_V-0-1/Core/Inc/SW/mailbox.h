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


#define  	MBOX_TIMEOUT		1
#define 	MBOX_NO_TIMEOUT		osWaitForever
#define 	MBOX_FROM_ISR		0

#define 	MBOX_SIZE			10


typedef enum c_type
{
	no_info,
	sleep,
	get_help,
	switch_rta,
	switch_ai,
	switch_settings,
	easteregg,
}cmd_t;



typedef struct {
	cmd_t type;
	entity_t origin;
	entity_t destination;
	prio_t prio;
	void* params;
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
	 * @params:		`uint8_t timeout`: timeout time, pass 0 if called from ISR
	 * @params:		`osThreadId_t rx_hTask`: handle of receiving task
	 * @params		`uint32_t flag`: notification for receiving task
	 * @returns 	`void`
	 * @brief:		push a command object to the mailbox's fifo buffer
	 * @notes:
	 * */
	void push(cmd cmd, uint8_t timeout, osThreadId_t rx_hTask, uint32_t flag);



	/* mailbox retrieval
	 * @params: 	`uint8_t timeout`: timeout time, pass 0 if called from ISR
	 * @returns 	`cmd`-object
	 * @brief:		acquire the oldest command in the buffer
	 * @notes:		**This function is blocking, only call it when data is available**
	 * */
	cmd pop(uint8_t timeout);



	/* check for data in queue
	 * @params: 	`void`
	 * @returns 	`uint_t`: # of messages in mailbox
	 * @brief:		peek into queue and check for pending messages
	 * @notes:		use this as the entry for `pop()`
	 * */
	uint8_t data_avail(void);



	/* delete all messages in mailbox
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:
	 * @notes:
	 * */
	void reset(void);



private:


};

// global references
extern mbox uart_mbox;

#endif
