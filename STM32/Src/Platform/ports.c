/*
 * ports.c
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */

#include "ports.h"

extern I2C_HandleTypeDef hi2c1;

/**
 * @brief: Initialize all base peripherals.
 */
void PortsInit(void)
{
	//Init Clock First.
	ClockInit();
	I2C_SetPort(&hi2c1);
	//MotorInit();
	ServoInit();
}
