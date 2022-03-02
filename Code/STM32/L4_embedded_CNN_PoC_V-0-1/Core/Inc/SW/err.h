#ifndef _ERR_H_
#define _ERR_H_

#include "main.h"

#define IS_NULL(x)	(x == NULL)

typedef enum type
{
	none,
	generic,
	busy,
	timeout,
	mem_null,
	no_such
}err_t;


typedef enum entity
{
	core_e,
	buffer_e,
	filterbank_e,
	ai_e,
	little_bash_e,
	display_e,
	mailbox_e,
	error_e
}entity_t;


typedef enum prio
{
	LOW,
	HIGH
}prio_t;



class err
{
public:

	err_t type = none;
	entity_t origin = core_e;
	prio_t prio = LOW;

	err(err_t t, entity_t o, prio_t p);

	void handle(void);
};


// not implemented
class warn
{
public:
	uint8_t warn;
	uint8_t origin;
	uint8_t sev;
};


#endif
