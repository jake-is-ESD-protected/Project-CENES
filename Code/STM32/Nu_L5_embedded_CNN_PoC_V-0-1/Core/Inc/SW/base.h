/*
*****************************************************************************************
file name:       	base.h
auth:               Jakob Tschavoll
date:               02.03.22
brief:              baseline general entities of device and their properties
version:            V0.1
notes:
*****************************************************************************************
*/


#ifndef _BASE_H_
#define _BASE_H_


/* [ENUM] entity_t
 * @brief:	list of entities in device
 * @intent:	make communication readable and hierarchic
 * */
typedef enum entity
{
	core_e,
	buffer_e,
	filterbank_e,
	ai_e,
	uTerm_e,
	nextion_e,
	mailbox_e,
	error_e,
	gpio_e
}entity_t;


/* [ENUM] prio_t
 * @brief:	priority list
 * @intent:	add a priority type to errors, mails, tasks, etc.
 * */
typedef enum prio
{
	LOW,
	HIGH
}prio_t;


#endif
