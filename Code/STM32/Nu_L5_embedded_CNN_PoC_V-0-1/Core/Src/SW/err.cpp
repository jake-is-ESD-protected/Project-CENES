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

error_handler e_handler;

error_handler::error_handler(void)
{

}



void error_handler::act(err_t e, entity_t origin)
{
	prio_t severity;

	switch(e)
	{
	case generic:
		severity = HIGH;
		break;

	case busy:
		severity = LOW;
		break;

	case timeout:
		severity = LOW;
		break;

	case mem_null:
		severity = HIGH;
		break;

	case delivery_fail:
		severity = LOW;
		break;

	case init_fail:
		severity = HIGH;
		break;

	case no_such:
		severity = HIGH;
		break;

	default:
		severity = NONE;
		break;
	}

	switch(severity)
	{
	case LOW:
		break;

	case HIGH:
		// throw "severe error";
		// TODO: try & catch
	default:
		break;
	}

}
