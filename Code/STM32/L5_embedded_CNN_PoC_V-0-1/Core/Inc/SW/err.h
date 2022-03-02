#ifndef _ERR_H_
#define _ERR_H_

#include "main.h"

typedef enum type
{
	none,
	generic,
	busy,
	timeout,
	mem_null,
}err_t;


typedef enum origin
{
	MAIN,
	RT,
	SW,
	UI
}origin_t;


typedef enum prio
{
	LOW,
	HIGH
}prio_t;



class err
{
public:

	err_t type;
	origin_t origin;
	prio_t prio;

	err(err_t t, origin_t o, prio_t p);

	void handle(void);
};


class warn
{
public:
	uint8_t warn;
	uint8_t origin;
	uint8_t sev;
};


#endif
