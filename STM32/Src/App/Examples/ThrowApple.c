/*
 * ThrowApple.c
 *
 *  Created on: 2021��8��27��
 *      Author: h13
 */
#include "ThrowApple.h"
#include "RobotConfigs.h"
//Arm Control
#include "ArmControl.h"
#include "Drivers.h"
#include "Ports.h"

void ThrowApple(void)
{
	uint16_t rotationAngle, axialLength, zAxisHeight;
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	if(zAxisHeight < ThrowAppleHeight)
	{
		for( ; zAxisHeight < ThrowAppleHeight; zAxisHeight ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(3);
		}
	} else {
		for( ; zAxisHeight > ThrowAppleHeight; zAxisHeight --)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(3);
		}
	}

	while(rotationAngle > 0)
	{
		SetOpenLoopClawPosition( -- rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(15);
	}

	if(axialLength < ThrowAppleAxialLength)
	{
		for( ; axialLength < ThrowAppleAxialLength; axialLength ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(15);
		}
	} else {
		for( ; axialLength > ThrowAppleAxialLength; axialLength --)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(15);
		}
	}

	ReleaseClaw();
}