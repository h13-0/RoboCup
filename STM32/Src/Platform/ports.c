/*
 * ports.c
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */

#include "ports.h"
#include "RobotConfigs.h"

extern I2C_HandleTypeDef I2C_Port;

/**
 * @brief: Initialize all base peripherals.
 */
void PortsInit(void)
{
	//Init Clock First.
	ClockInit();
	I2C_SetPort(&I2C_Port);
}
