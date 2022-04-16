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
	SetAL_AxisLength(AL_AxisMaximumLength);

	SleepMillisecond(300);

	SmoothRotateArmNode(ClawRotation, 90, 5);

	SmoothRotateArmNode(ArmParallel, 80, 7);

	SleepMillisecond(1000);

	SmoothRotateArmNode(ClawRotation, 0, 5);

	SmoothRotateArmNode(ArmRotation, 0, 15);

	SmoothRotateArmNode(ArmElongation, 100, 10);

	SmoothRotateArmNode(ArmParallel, 154, 10);

	SetAL_AxisLength(AL_AxisMinimumLength);
}
