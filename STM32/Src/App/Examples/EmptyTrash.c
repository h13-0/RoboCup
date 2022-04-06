/*
 * EmptyTrash.c
 *
 *  Created on: 2021Äê8ÔÂ28ÈÕ
 *      Author: h13
 */
#include "EmptyTrash.h"
//Arm Control
#include "ArmControl.h"
#include "Drivers.h"
#include "Ports.h"

void EmptyTrash(void)
{
	SmoothRotateArmNode(ArmRotation, 45, 15);
	SleepMillisecond(300);

	SmoothRotateArmNode(ArmElongation, 170, 10);

	SleepMillisecond(1000);

	SmoothRotateArmNode(ArmRotation, 0, 15);
}
