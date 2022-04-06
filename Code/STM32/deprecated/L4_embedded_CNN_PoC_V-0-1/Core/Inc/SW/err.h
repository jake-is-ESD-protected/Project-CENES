/*
*****************************************************************************************
file name:       	err.h
auth:               Jakob Tschavoll
date:               27.02.22
brief:              errors as objects
version:            V0.1
notes:
*****************************************************************************************
*/


#ifndef _ERR_H_
#define _ERR_H_

#include "main.h"
#include "base.h"

#define IS_NULL(x)	(x == NULL)



/* [ENUM] err_t
 * @brief:	error type
 * @intent:	make errors readable
 * */
typedef enum type
{
	none,
	generic,
	busy,
	timeout,
	mem_null,
	delivery_fail,
	no_such
}err_t;


/* [CLASS] err
 * @brief:	specific error implementation
 * @intent:	handle all errors over a case-specific error handler
 * */
class err
{
public:

	err_t type = none;
	entity_t origin = core_e;
	prio_t prio = LOW;
	void* description = NULL;

	uint32_t translate = *((uint32_t*)description);




	/* error-constructor
	 * @params: 	`err_t t`: type of error
	 * @params:		`entity_t o`: place of origin
	 * @params: 	`prio_t p`: priority
	 * @params: 	`void* desc`: description
	 * @returns:	`err`-object
	 * @brief:		`create a specific error`
	 * @notes:
	 * */
	err(err_t t, entity_t o, prio_t p, void* desc);



	/* error-handler
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		handle the created error (this)
	 * @notes:		information stored in public variables
	 * */
	void handle(void);
};

#endif
