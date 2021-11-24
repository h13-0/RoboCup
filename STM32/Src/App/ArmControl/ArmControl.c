/*
 * ArmControl.c
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */
#include "ArmControl.h"
#include "RobotConfigs.h"
#include "Servo.h"
#include "Clock.h"

/**
 * @brief: Closure the claw.
 * @TODO:  Use current to judge whether it is firmly grasped.
 */
void ClosureClaw(void)
{
	SetArmNodeAngle(ClawGrab, MaximumRotationAngleOfGraspingApple);
}

/**
 * @brief: Release the claw.
 */
void ReleaseClaw(void)
{
	SetArmNodeAngle(ClawGrab, 180);
}

/**
 * @brief: Smooth move claw to position in **Cylindrical coordinates** in Open loop control system.
 * @param:
 * 		TargetCoordinates:       Target control coordinates.
 * 		TargetValue:             Target value.
 * 		MillisecondDelayPerStep: Milliseconds per mm / degree delay.
 */
void SmoothMoveTo(TargetCoordinates_t TargetCoordinates, uint16_t TargetValue, mtime_t MillisecondDelayPerStep)
{
	uint16_t rotationAngle = 0, axialLength = 0, zAxisHeight = 0;
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);
	switch (TargetCoordinates) {
		case MoveRotationAngle:
			while(TargetValue != rotationAngle)
			{
				if(rotationAngle < TargetValue)
				{
					rotationAngle ++;
				} else {
					rotationAngle --;
				}
				SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
				SleepMillisecond(MillisecondDelayPerStep);
			}
			break;

		case MoveAxialLength:
			while(TargetValue != axialLength)
			{
				if(axialLength < TargetValue)
				{
					axialLength ++;
				} else {
					axialLength --;
				}
				SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
				SleepMillisecond(MillisecondDelayPerStep);
			}
			break;

		case MoveZ_AxisHeight:
			while(TargetValue != zAxisHeight)
			{
				if(zAxisHeight < TargetValue)
				{
					zAxisHeight ++;
				} else {
					zAxisHeight --;
				}
				SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
				SleepMillisecond(MillisecondDelayPerStep);
			}
			break;

		default:
			break;
	}
}
