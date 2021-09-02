/*
 * Drivers.c
 *
 *  Created on: 2021Äê9ÔÂ2ÈÕ
 *      Author: h13
 */
#include "Drivers.h"

/**
 * @brief: Init all drivers.
 */
void DriversInit(void)
{
	MotorInit();
	ServoInit();
}
