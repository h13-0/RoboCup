/*
 * CatchApple.c
 *
 *  Created on: 2021Äê8ÔÂ14ÈÕ
 *      Author: h13
 */
#include "CatchApple.h"

//Arm Control
#include "ArmControl.h"

void CatchApple(void)
{
	ReleaseClaw();

	for(uint16_t times = 0; times < 330; times ++)
	{
		SetOpenLoopClawPosition(0, 0, times);
		SleepMillisecond(3);
	}

	for(uint16_t times = 0; times < 90; times ++)
	{
		SetOpenLoopClawPosition(times, 0, 330);
		SleepMillisecond(5);
	}

	for(uint16_t times = 0; times < 110; times ++)
	{
		SetOpenLoopClawPosition(90, times, 330);
		SleepMillisecond(5);
	}

	//Wait imageprocessing module prepared.
	SleepMillisecond(10000);

	AimAtApple(10, 20000);

	float rotationAngle, axialLength, zAxisHeight;
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	for( ; zAxisHeight >= 255; zAxisHeight--)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(5);
	}

	SleepMillisecond(300);

	for(uint16_t times = 0; times < 55; times ++)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength++, zAxisHeight);
		SleepMillisecond(20);
	}

	SleepMillisecond(300);

	ClosureClaw();
}

