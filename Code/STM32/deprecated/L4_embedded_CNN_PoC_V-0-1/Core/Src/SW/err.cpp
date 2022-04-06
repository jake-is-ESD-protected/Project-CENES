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


#include "err.h"

err::err(err_t t, entity_t o, prio_t p, void* desc)
{
	this->type = t;
	this->origin = o;
	this->prio = p;
	this->description = desc;
}



void err::handle(void)
{

	// TODO

}
