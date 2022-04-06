/*
*****************************************************************************************
file name:       	gpio.cpp
auth:               Jakob Tschavoll
date:               03.03.22
brief:              gpios as objects
version:            V0.1
notes:
*****************************************************************************************
*/

#include "gpio.h"

// singleton instance
gpio gpio_D9;
gpio gpio_D7;
gpio gpio_D6;

gpio::gpio(void)
{

}



void gpio::init(GPIO_TypeDef* grp, uint16_t pn)
{
	this->group = grp;
	this->pin = pn;
}



void gpio::set(bool state)
{
	HAL_GPIO_WritePin(group, pin, (GPIO_PinState)state);
}
