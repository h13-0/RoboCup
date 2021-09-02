/*
 * CatchApple.c
 *
 *  Created on: 2021��8��14��
 *      Author: h13
 */
#include "ArmControl.h"
#include "CatchApple.h"
#include "RobotConfigs.h"
#include "Drivers.h"
//Arm Control
#include "ImageProcessingModule.h"

//TODO: Bug entering preparation posture��
void CatchApple(TargetType_t Target)
{
	//Enter the ready position.
	ReleaseClaw();

	SetArmNodeAngle(ClawRotation, 0);

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

	if(axialLength < BaseAxialLength)
	{
		for( ; axialLength < BaseAxialLength; axialLength ++)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	} else {
		for( ; axialLength > BaseAxialLength; axialLength --)
		{
			SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
			SleepMillisecond(5);
		}
	}

	//Switching the working mode of image processing module.
	switch(Target)
	{
	case MaximumTarget:
		SwitchImageProcessingModuleWorkingMode(AppleDetectMax);
		break;

	case LeftTarget:
		SwitchImageProcessingModuleWorkingMode(AppleDetectLeft);
		break;

	case RightTarget:
		SwitchImageProcessingModuleWorkingMode(AppleDetectRight);
		break;

	default:
		break;
	}

	//Aim at apple.
	AimAt(Apple, 20000);

	//Catch apple.
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	//Approaching apple.
	for( ; zAxisHeight > GrabHeight; zAxisHeight--)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(15);
	}

	SleepMillisecond(300);

	for(uint16_t times = 0; times < ElongationDistance; times ++)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength++, zAxisHeight);
		SleepMillisecond(20);
	}

	SleepMillisecond(300);

	//Catch.
	ClosureClaw();

	SleepMillisecond(500);

	//Arm retraction.
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);

	for( ; zAxisHeight < RetractionHeight; zAxisHeight++)
	{
		SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
		SleepMillisecond(5);
	}

#if(ArmType == MechanicalArm)
	SmoothRotateArmNode(ArmParallel, -45, 15);
	SmoothRotateArmNode(ArmElongationNode0, 45, 15);
	SmoothRotateArmNode(ArmElongationNode1, 0, 15);
	SmoothRotateArmNode(ArmRotation, 0, 15);
	SmoothRotateArmNode(ArmParallel, 0, 15);
	SmoothRotateArmNode(ArmElongationNode0, 0, 15);

#elif(ArmType == LiftingPlatform)

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
#endif
}

