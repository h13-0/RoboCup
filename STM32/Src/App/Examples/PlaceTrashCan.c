/*
 * PlaceTrashCan.c
 *
 *  Created on: 2021Äê8ÔÂ28ÈÕ
 *      Author: h13
 */
#include "PlaceTrashCan.h"
//Arm Control
#include "ArmControl.h"
#include "Drivers.h"
#include "Ports.h"

void PlaceTrashCan(void)
{
	SetAL_AxisLength(150);
	SleepMillisecond(200);

	SmoothRotateArmNode(ArmParallel, 154, 10);
	SmoothRotateArmNode(ArmElongation, 80, 10);
	SleepMillisecond(500);
	ReleaseClaw();
	SleepMillisecond(500);

	SmoothRotateArmNode(ArmParallel, 0, 10);
	SmoothRotateArmNode(ArmElongation, 0, 10);
	SetAL_AxisLength(AL_AxisMinimumLength);
	SleepMillisecond(200);

	//Switch mode to openloop
	SwitchArmControlMode(OpenLoopMode);
}
