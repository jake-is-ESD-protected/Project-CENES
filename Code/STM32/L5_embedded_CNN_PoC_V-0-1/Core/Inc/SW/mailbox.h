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


#define  	QUEUE_TIMEOUT	1
#define 	QUEUE_FROM_ISR	0


typedef enum c_type
{
	no_info,
	sleep,
	get_help,
	switch_rta,
	switch_ai,
}cmd_t;


// NOTE: for ? reason, mail queues are not available, which means that
// only messages of type `uint32_t` can be sent. This makes the mailbox
// system not object-proof and a struct of `4x8` bits will have to
// suffice. Pay attention to typecasting and de-typecasting. Also, the
// values should be filled with above enums, even though the datatype
// seems to be different. Treat them as `#defines`
typedef struct {
	uint8_t type;
	uint8_t origin;
	uint8_t prio;
	uint8_t unused;
}cmd;



class mbox
{
public:

	osMessageQDef(queue, 10, uint32_t);
	osMessageQId queue_id;


	/* mailbox-constructor
	 * @params: 	`void`
	 * @returns 	`mbox`-object
	 * @brief:		Init a queue wrapper
	 * @notes:
	 * */
	mbox(osThreadId_t receiving_thread);



	/* mailbox insertion
	 * @params: 	`cmd`-struct
	 * @params:		`bool from_ISR`: specify if mbox is filled inside an ISR
	 * @returns 	`void`
	 * @brief:		push a command object to the mailbox's fifo buffer
	 * @notes:
	 * */
	void push(cmd cmd, bool from_ISR);



	/* mailbox retrieval
	 * @params: 	`void`
	 * @returns 	`cmd`-object
	 * @brief:		acquire the oldest command in the buffer
	 * @notes:		popping a command consumes it
	 * */
	cmd pop(void);



private:


};

#endif
