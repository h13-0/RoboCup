/*
 * CatchTrashCan.c
 *
 *  Created on: 2021Äê8ÔÂ28ÈÕ
 *      Author: h13
 */
#include "CatchTrashCan.h"
#include "RobotConfigs.h"
//Arm Control
#include "ArmControl.h"
#include "Drivers.h"
#include "Ports.h"
#include "Examples.h"

void CatchTrashCan(void)
{
	ReleaseClaw();

	//Enter the ready position.
	PrepareArmPosition(87);

	SwitchArmControlMode(ManualMode);

	SetArmNodeAngle(ClawRotation, 0);

	//AS215
	SetAL_AxisLength(215);

	SleepMillisecond(200);

	//AEN121
	SmoothRotateArmNode(ArmElongation, 121, 10);

	//APN154
	SmoothRotateArmNode(ArmParallel, 154, 10);

	//AS170
	SetAL_AxisLength(170);
	SleepMillisecond(200);

	//AEN60
	SmoothRotateArmNode(ArmElongation, 60, 10);

	SleepMillisecond(500);

	//Catch
	ClosureClaw();

	SleepMillisecond(750);

	//AEN100
	SmoothRotateArmNode(ArmElongation, 100, 10);

	//APN154
	SmoothRotateArmNode(ArmParallel, 154, 10);

	//AS0
	SetAL_AxisLength(AL_AxisMinimumLength);
}
