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
	generic,
	busy,
	timeout,
	mem_null,
	delivery_fail,
	init_fail,
	no_such
}err_t;


/* [CLASS] err
 * @brief:	specific error implementation
 * @intent:	handle all errors over a case-specific error handler
 * */
class error_handler
{
public:

	/* handler-constructor
	 * @params: 	`void`
	 * @returns:	`error_handler`-object
	 * @brief:		`create a universal error handler`
	 * @notes:
	 * */
	error_handler(void);



	/* error-handler
	 * @params: 	`err_t e`: error to handle
	 * @params: 	`entity_t origin`: information about throwing entity
	 * @returns 	`void`
	 * @brief:		handle the created error
	 * @notes:		point of exit. This raises an exception and stalls operation
	 * */
	void act(err_t e, entity_t origin);
};

// singleton reference
extern error_handler e_handler;

#endif
