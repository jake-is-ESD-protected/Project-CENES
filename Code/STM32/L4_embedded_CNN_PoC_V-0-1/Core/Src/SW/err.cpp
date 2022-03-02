#include "err.h"

err::err(err_t t, entity_t o, prio_t p)
{
	this->type = t;
	this->origin = o;
	this->prio = p;
}



void err::handle(void)
{

	// TODO

}
