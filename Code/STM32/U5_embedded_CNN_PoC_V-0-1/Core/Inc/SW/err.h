#ifndef _ERR_H_
#define _ERR_H_

#include "main.h"

class err
{
public:
	uint8_t type;
	uint8_t origin;
	uint8_t sev;

	err(HAL_StatusTypeDef type);

	void hdler_generic(void);
};


class warn
{
public:
	uint8_t warn;
	uint8_t origin;
	uint8_t sev;
};


#endif
