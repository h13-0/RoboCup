/*
 * WashAple.c
 *
 *  Created on: 2021Äê8ÔÂ24ÈÕ
 *      Author: h13
 */
#include "ArmControl.h"
#include "WashApple.h"
#include "PrepareArmPosition.h"
#include "RobotConfigs.h"

#include "Ports.h"
#include "Drivers.h"

#if(ArmType == MechanicalArm)
/**
 * @brief: Wash Apple
 */
void WashApple(void)
{
	uint16_t rotationAngle, axialLength, zAxisHeight;
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	if(zAxisHeight < WashAppleHeight)
	{
		for( ; zAxisHeight < WashAppleHeight; zAxisHeight ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(3);
		}
	} else {
		for( ; zAxisHeight > WashAppleHeight; zAxisHeight --)
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

	if(axialLength < WashAppleAxialLength)
	{
		for( ; axialLength < WashAppleAxialLength; axialLength ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	} else {
		for( ; axialLength > WashAppleAxialLength; axialLength --)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	}

	for(uint8_t angle = 0; angle < 180; angle++)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	for(uint8_t angle = 180; angle > 0; angle--)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	for(uint8_t angle = 0; angle < 180; angle++)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	for(uint8_t angle = 180; angle > 0; angle--)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	//Arm retraction.
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	for( ; axialLength > 0; axialLength --)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(5);
	}

	for( ; rotationAngle > 0; rotationAngle --)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(5);
	}

	for( ; zAxisHeight > 0; zAxisHeight --)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(5);
	}
}
#elif(ArmType == LiftingPlatform)
/**
 * @brief: Wash Apple
 */
void WashApple(void)
{
	SmoothMoveTo(MoveRotationAngle, 90, 15);
	SmoothMoveTo(MoveAxialLength, 300, 5);

	for(uint8_t angle = 0; angle < 180; angle++)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	for(uint8_t angle = 180; angle > 0; angle--)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	for(uint8_t angle = 0; angle < 180; angle++)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	for(uint8_t angle = 180; angle > 0; angle--)
	{
		SetArmNodeAngle(ClawRotation, angle);
		SleepMillisecond(5);
	}

	PrepareArmPosition(0);
}
#endif
