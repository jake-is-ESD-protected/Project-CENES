/*
*****************************************************************************************
file name:       	gpio.h
auth:               Jakob Tschavoll
date:               03.03.22
brief:              gpios as objects
version:            V0.1
notes:
*****************************************************************************************
*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "main.h"
#include "err.h"

class gpio
{
public:

	GPIO_TypeDef* group = NULL;
	uint16_t pin = 0;



	/* gpio-constructor
	 * @params: 	`void`
	 * @returns 	`gpio`-object
	 * @brief:
	 * @notes:
	 * */
	gpio(void);



	/* gpio-constructor
	 * @params: 	`uint8_t grp`: GPIO group as specified by board
	 * @params:		`uint8_t pin`: GPO pin as specified by board
	 * @returns 	`void`
	 * @brief:
	 * @notes:
	 * */
	void init(GPIO_TypeDef* grp, uint16_t pn);



	/* gpio-constructor
	 * @params: 	`bool state`: state of gpio
	 * @returns 	`void`
	 * @brief:
	 * @notes:
	 * */
	void set(bool state);

};

// singleton reference
extern gpio gpio_D3;
extern gpio gpio_D2;
extern gpio gpio_D4;

#endif
