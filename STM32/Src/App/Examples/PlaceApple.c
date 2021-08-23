/*
 * PlaceApple.c
 *
 *  Created on: 2021Äê8ÔÂ23ÈÕ
 *      Author: h13
 */

#include "PlaceApple.h"
#include "RobotConfigs.h"

#include "ArmControl.h"
#include "Ports.h"
#include "Drivers.h"

#include <stdint.h>

/**
 * @brief: Place the apple in the center of the target.
 * @TODO:  Place in the center of the target.
 */
void PlaceApple(void)
{
	float rotationAngle, axialLength, zAxisHeight;
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
