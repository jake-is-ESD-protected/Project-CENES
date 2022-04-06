/*
*****************************************************************************************
file name:       	core.h
auth:               Jakob Tschavoll
date:               25.02.22
brief:              Central FSM, connects UI with RT & SW
version:            V0.1
notes:
*****************************************************************************************
*/

#ifndef _CORE_H_
#define _CORE_H_


#include "main.h"

class core
{
public:
	typedef enum state
	{
		init,
		idle,
		rta,
		ai
	}state_t;



};



#endif
