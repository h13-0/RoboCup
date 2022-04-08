/*
 * PlaceApple.c
 *
 *  Created on: 2021Äê8ÔÂ23ÈÕ
 *      Author: h13
 */

#include "ArmControl.h"
#include "PlaceApple.h"
#include "RobotConfigs.h"

#include "Ports.h"
#include "Drivers.h"

#include <stdint.h>

#if(ArmType == MechanicalArm)
/**
 * @brief: Place the apple in the center of the target.
 * @TODO:  Place in the center of the target.
 */
void PlaceApple(void)
{
	uint16_t rotationAngle, axialLength, zAxisHeight;
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	if(zAxisHeight < ApproachHeight)
	{
		for( ; zAxisHeight < ApproachHeight; zAxisHeight ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(3);
		}
	} else {
		for( ; zAxisHeight > ApproachHeight; zAxisHeight --)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(3);
		}
	}

	if(rotationAngle < 90)
	{
		for( ; rotationAngle < 90; rotationAngle ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	} else {
		for( ; rotationAngle > 90; rotationAngle --)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	}

	if(axialLength < 350)
	{
		for( ; axialLength < 350; axialLength ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	} else {
		for( ; axialLength > 350; axialLength --)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	}

	for( ; zAxisHeight > GrabHeight; zAxisHeight--)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(15);
	}

	ReleaseClaw();

	SleepMillisecond(200);

	for( ; zAxisHeight < ApproachHeight; zAxisHeight++)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(5);
	}

	for( ; axialLength > BaseAxialLength; axialLength --)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(5);
	}
}
#elif(ArmType == LiftingPlatform)
/**
 * @brief:  Place the apple in the center of the target.
 * @return: RotationAngle of the target.
 */
uint16_t PlaceApple(void)
{
	//Enter the ready position.
	ClosureClaw();
	SetArmNodeAngle(ClawRotation, 0);

	SmoothMoveTo(MoveRotationAngle, 90, 15);
	SmoothMoveTo(MoveAxialLength, 221, 5);
	SmoothMoveTo(MoveZ_AxisHeight, ApproachHeight, 5);

	// Take a snapshot to debug.
#ifdef DEBUG
	SleepMillisecond(500);
	TakeSnapShot();
#endif

	SwitchImageProcessingModuleWorkingMode(TargetDetection);

	AimAt(AimTarget, 20000);

	uint16_t rotationAngle = 0, axialLength = 0, zAxisHeight = 0;
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	SmoothMoveTo(MoveAxialLength, axialLength + PlaceAppleElongationDistance, 5);
	SmoothMoveTo(MoveZ_AxisHeight, GrabHeight, 5);

	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);
	while(IsZ_AxisBusy());
	SleepMillisecond(500);

	ReleaseClaw();

	SleepMillisecond(500);

	SmoothMoveTo(MoveZ_AxisHeight, ApproachHeight, 5);
	while(IsZ_AxisBusy());

	SleepMillisecond(200);

	SmoothMoveTo(MoveAxialLength, 221, 10);
	SmoothMoveTo(MoveRotationAngle, 90, 15);
	return rotationAngle;
}
#endif
