/*
 * MotionControl.c
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */
#include "MotionControl.h"
#include "RobotConfigs.h"

#include <stdlib.h>
#include <math.h>

#include "Drivers.h"

#include "PID.h"
#include "ports.h"

#ifdef DEBUG
//Share PID Structure with ParameterAdjust.c
PositionPID_t AnglePID;
PositionPID_t ForwardPID;
PositionPID_t LeftSpeedPID, RightSpeedPID;
#define anglePID      AnglePID
#define forwardPID    ForwardPID
#define leftSpeedPID  LeftSpeedPID
#define rightSpeedPID RightSpeedPID
#else
static PositionPID_t anglePID;
static PositionPID_t forwardPID;
static PositionPID_t leftSpeedPID, rightSpeedPID;
#endif

/**
 * @group: PID Status Variable
 * @Bits:
 * 		Bit0: Status of direction PID.
 * 				Enable   -> 1.
 * 				Disable  -> 0.
 * 		Bit1: Status of forward PID.
 * 				Enable   -> 1.
 * 				Disable  -> 0.
 */

struct motionControlStatusStructure
{
	uint8_t speedPID_Status     : 1;
	uint8_t directionPID_Status : 1;
	uint8_t forwardPID_Status   : 1;
};

static struct motionControlStatusStructure status = { 0 };

//Status of speed PID.
#define enableSpeedPID()              status.speedPID_Status = 1
#define disableSpeedPID()             disableForwardPID(); disableDirectionPID(); status.speedPID_Status = 0; leftSpeedPID._sumError = 0; rightSpeedPID._sumError = 0; Brake();
#define getSpeedPID_Status()          status.speedPID_Status
//Status of direction PID.
#define enableDirectionPID()          status.directionPID_Status = 1
#define disableDirectionPID()         status.directionPID_Status = 0; anglePID._sumError = 0; speedDifference = 0
#define getDirectionPID_Status()      status.directionPID_Status
//Status of forward PID.
#define enableForwardPID()            status.forwardPID_Status = 1
#define disableForwardPID()           status.forwardPID_Status = 0; forwardPID._sumError = 0; speedBaseOutput = 0
#define getForwardPID_Status()        status.forwardPID_Status

float leftPWM = 0, rightPWM = 0;
static int32_t leftSpeed  = 0;
static int32_t rightSpeed = 0;

/**
 * @group: Direction Variables.
 */
//
static float yaw = 0;

/**
 * @group: Forward Variables.
 */
static float distance = 0.0;

/**
 * @TODO:
 */
static float speedDifference = 0;
static float speedBaseOutput = 0;

/**
 * @brief: Init motion control.
 * @note:
 * 		Before you can use this, you need to initialize Motor and MPU manually ahead of time.
 * 		It will Init Direction PID, turn to zero and brake.
 */
void MotionControlInit(void)
{
	//speed PID configs.
	leftSpeedPID.proportion = SpeedPID_Proportion;
	leftSpeedPID.integration = SpeedPID_Integration;
	leftSpeedPID.differention = SpeedPID_Differention;
	leftSpeedPID.setpoint = 0.0;
	leftSpeedPID.maxAbsOutput = GetMaxValueOfPWM();

	leftSpeedPID.configs.autoResetIntegration = disable;
	leftSpeedPID.configs.limitIntegration = enable;
	leftSpeedPID.maximumAbsValueOfIntegrationOutput = GetMaxValueOfPWM();

	rightSpeedPID = leftSpeedPID;

	//@TODO: Verify that autoResetIntegration needs to be turned off
	//angle PID configs.
	anglePID.proportion = AnglePID_Proportion;
	anglePID.integration = AnglePID_Integration;
	anglePID.differention = AnglePID_Differention;
	anglePID.setpoint = 0.0;
	anglePID.maxAbsOutput = MaxSpeed * 0.3;

	//Extend functions config.
	anglePID.configs.autoResetIntegration = disable;
	anglePID.configs.limitIntegration = enable;
	anglePID.maximumAbsValueOfIntegrationOutput = MaxSpeed * 0.1;

	forwardPID.proportion = ForwardPID_Proportion;
	forwardPID.integration = ForwardPID_Integration;
	forwardPID.differention = ForwardPID_Differention;
	forwardPID.setpoint = 0.0;
	forwardPID.maxAbsOutput = MaxSpeed * 0.70;

	//Extend functions config.
	forwardPID.configs.autoResetIntegration = disable;
	forwardPID.configs.limitIntegration = enable;
	forwardPID.maximumAbsValueOfIntegrationOutput = MaxSpeed * 0.3;

	enableSpeedPID();
}

/**
 * @brief: Calculate the minimum difference between the TargetYaw and the CurrentYaw.
 * @param:
 * 		CurrentYaw: Current yaw in range: (-180, 180)
 * 		TargetYaw:  Target yaw in range: (-180, 180)
 * @example:
 * 		calculateYawError(40, 0)     -> -40
 * 		calculateYawError(40, -40)   -> -80
 * 		calculateYawError(-40, 40)   -> 80
 * 		calculateYawError(-180, 180) -> 0
 * @note:   Clockwise is positive.
 */
static float calculateYawError(float CurrentYaw, float TargetYaw)
{
	float error = 0;
	//Preprocess parameters.
	while(CurrentYaw > 180)
	{
		CurrentYaw -= 360;
	}

	while(CurrentYaw < -180)
	{
		CurrentYaw += 360;
	}

	while(TargetYaw > 180)
	{
		TargetYaw -= 360;
	}

	while(TargetYaw < -180)
	{
		TargetYaw += 360;
	}

	//baseError = TargetYaw - CurrentYaw
	//Then baseError in (-360, 360).
	//baseError in (-360, -180) -> clockwise
	//baseError in (-180, 0)    -> antiClockwise
	//baseError in (0, 180)     -> closkwise
	//baseError in (180, 360)   -> antiClockwise
	//if(clockwiseError > antiClockwiseError)
	if(((TargetYaw - CurrentYaw < 180.0) && (TargetYaw - CurrentYaw > 0.0)) || (TargetYaw - CurrentYaw < - 180.0))
	{
		//Turn clockwise, error < 0.
		//if(Cross ¡À180 degrees)
		if(CurrentYaw > 0 && TargetYaw < 0)
		{
			error = CurrentYaw - TargetYaw - 360;
		} else {
			error = CurrentYaw - TargetYaw;
		}
	} else {
		//Turn anti-clockwise, error > 0.
		//if(Cross ¡À180 degrees)
		if(CurrentYaw < 0 && TargetYaw > 0)
		{
			error = CurrentYaw - TargetYaw + 360;
		} else {
			error = CurrentYaw - TargetYaw;
		}
	}
	return error;
}

/**
 * @brief: Turn To ..
 * @param:
 * 		Direction: The direction you want to turn to.
 */
void TurnTo(direction_t Direction)
{
	switch(Direction)
	{
	case Left:
		anglePID.setpoint -= 90;
		break;

	case Right:
		anglePID.setpoint += 90;
		break;

	case BackWard:
		anglePID.setpoint += 180;
		break;

	default:
		return;
	}

	while(anglePID.setpoint < -180)
	{
		anglePID.setpoint += 360;
	}

	while(anglePID.setpoint > 180)
	{
		anglePID.setpoint -= 360;
	}

	enableSpeedPID();
	enableDirectionPID();

	uint32_t startTime = GetCurrentTimeMillisecond();
	uint32_t startStableTime = GetCurrentTimeMillisecond();

	while(1)
	{
		leftSpeedPID.setpoint = speedBaseOutput + speedDifference;
		rightSpeedPID.setpoint = speedBaseOutput - speedDifference;

		if(fabs(calculateYawError(yaw, anglePID.setpoint)) > AngleAccurary)
		{
			startStableTime = GetCurrentTimeMillisecond();
		}

		if(GetCurrentTimeMillisecond() - startStableTime > AngleStableTimeThreshold)
		{
			break;
		}

		if(GetCurrentTimeMillisecond() - startTime > AngleAdjustTimeLimit)
		{
			break;
		}
	}

	disableDirectionPID();
	disableSpeedPID();
	Brake();
}

void StraightUntill(uint16_t Distance)
{
	forwardPID.setpoint = Distance;

	enableSpeedPID();
	enableDirectionPID();
	enableForwardPID();

	mtime_t startTime = GetCurrentTimeMillisecond();
	mtime_t startStableTime = GetCurrentTimeMillisecond();
	mtime_t startTrimTime = GetCurrentTimeMillisecond();

	while (1)
	{
		leftSpeedPID.setpoint = speedBaseOutput + speedDifference;
		rightSpeedPID.setpoint = speedBaseOutput - speedDifference;

		if((abs(Distance - distance) < ForwardTrimDistance) && (fabs(calculateYawError(yaw, anglePID.setpoint)) < ForwardTrimAngleErrorLimit))
		{
			disableDirectionPID();
			if(GetCurrentTimeMillisecond() - startTrimTime > ForwardTrimTimeLimit)
			{
				break;
			}
		} else {
			enableDirectionPID();
			startTrimTime = GetCurrentTimeMillisecond();
		}

		if(abs(distance - forwardPID.setpoint) > ForwardAccuracy)
		{
			startStableTime = GetCurrentTimeMillisecond();
		}

		if(GetCurrentTimeMillisecond() - startStableTime > ForwardStableTimeThreshold)
		{
			break;
		}

		if(GetCurrentTimeMillisecond() - startTime > ForwardAdjustTimeLimit)
		{
			break;
		}
	}

	leftSpeedPID.setpoint = 0;
	rightSpeedPID.setpoint = 0;
	disableForwardPID();
	disableDirectionPID();
	disableSpeedPID();
}

#ifdef DEBUG

/**
 * @brief: Keep Angle To Adjust PID Value.
 * @note:
 * 		**Blocking function!!!**
 * 		Just for PID Adjust!!!
 */
void KeepSpeed(void)
{
	enableSpeedPID();
	while(1)
	{
		LogJustFloat(leftSpeed, rightSpeed, leftPWM, rightPWM, leftSpeedPID.setpoint, rightSpeedPID.setpoint);
		SleepMillisecond(10);
	}
	disableSpeedPID();
}

void KeepAngle(void)
{
	enableSpeedPID();
	enableDirectionPID();
	while (1)
	{
		leftSpeedPID.setpoint = speedBaseOutput + speedDifference;
		rightSpeedPID.setpoint = speedBaseOutput - speedDifference;

		LogJustFloat(yaw, anglePID.setpoint, GetYawVelocity(), speedDifference, leftPWM, rightPWM);

		SleepMillisecond(10);
	}
	disableDirectionPID();
	disableSpeedPID();
}

void KeepDistance(uint16_t Distance)
{
	forwardPID.setpoint = Distance;
	enableSpeedPID();
	enableDirectionPID();
	enableForwardPID();

	while(1)
	{
		leftSpeedPID.setpoint = speedBaseOutput + speedDifference;
		rightSpeedPID.setpoint = speedBaseOutput - speedDifference;
		LogJustFloat(distance, yaw, forwardPID.setpoint, speedBaseOutput, speedDifference);
		SleepMillisecond(10);
	}

	disableForwardPID();
	disableDirectionPID();
	disableSpeedPID();
}

#endif

/**
 * @group: PID Calculate Handler
 */
__attribute__((always_inline)) inline void SpeedPIDCalculateHandler(void)
{
	leftSpeed  = GetLeftEncoderSpeed();
	rightSpeed = GetRightEncoderSpeed();

	if(getSpeedPID_Status())
	{
		leftPWM = PosPID_Calc(&leftSpeedPID, leftSpeed);
		rightPWM = PosPID_Calc(&rightSpeedPID, rightSpeed);

		SetLeftMotorPWM(leftPWM);
		SetRightMotorPWM(rightPWM);
	}
}

__attribute__((always_inline)) inline void DirectionPIDCalculateHandler(void)
{
	if(getDirectionPID_Status())
	{
		yaw = GetYawValue();
		speedDifference = PosPID_CalcWithCustErrAndDiff(&anglePID, calculateYawError(yaw, anglePID.setpoint), GetYawVelocity());
	}
}

__attribute__((always_inline)) inline void ForwardPIDCalculateHandler(void)
{
	if(getForwardPID_Status())
	{
		distance = GetTOF_Distance();
		speedBaseOutput = PosPID_Calc(&forwardPID, distance);
	}
}
