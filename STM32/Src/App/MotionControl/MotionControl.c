/*
 * MotionControl.c
 *
 *  Created on: 2021��6��8��
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
static PositionPID_t speedPID;
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
#define disableSpeedPID()             status.speedPID_Status = 0
#define getSpeedPID()                 status.speedPID_Status
//Status of direction PID.
#define enableDirectionPID()          status.directionPID_Status = 1
#define disableDirectionPID()         status.directionPID_Status = 0
#define getDirectionPIDStatus()       status.directionPID_Status
//Status of forward PID.
#define enableForwardPID()            status.forwardPID_Status = 1
#define disableForwardPID()           status.forwardPID_Status = 0
#define getForwardPIDStatus()         status.forwardPID_Status

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
	anglePID.proportion = 0.6;
	anglePID.integration = 0.12;
	anglePID.differention = 0.10;
	anglePID.setpoint = 0.0;
	anglePID.maxAbsOutput = fmax(13.5, MaxSpeed * 0.5);

	//Extend functions config.
	anglePID.configs.autoResetIntegration = disable;
	anglePID.configs.limitIntegration = enable;
	anglePID.maximumAbsValueOfIntegrationOutput = MaxSpeed * 0.1;

	forwardPID.proportion = -0.14;
	forwardPID.integration = 0.0;
	forwardPID.differention = 0.0;
	forwardPID.setpoint = 0.0;
	forwardPID.maxAbsOutput = MaxSpeed * 0.7;

	//Extend functions config.
	forwardPID.configs.autoResetIntegration = disable;
	forwardPID.configs.limitIntegration = enable;
	forwardPID.maximumAbsValueOfIntegrationOutput = MaxSpeed * 0.3;

	//enableSpeedPID();
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
		//if(Cross ��180 degrees)
		if(CurrentYaw > 0 && TargetYaw < 0)
		{
			error = CurrentYaw - TargetYaw - 360;
		} else {
			error = CurrentYaw - TargetYaw;
		}
	} else {
		//Turn anti-clockwise, error > 0.
		//if(Cross ��180 degrees)
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

	//Keep Angle
	anglePID._sumError = 0;
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
	anglePID._sumError = 0;
	Brake();
}

void StraightUntill(uint16_t Distance)
{
	//Keep Angle
	enableDirectionPID();

	forwardPID.setpoint = Distance;
	enableForwardPID();

	uint32_t startTime = GetCurrentTimeMillisecond();
	uint32_t startStableTime = GetCurrentTimeMillisecond();

	while (1)
	{
		leftSpeedPID.setpoint = speedBaseOutput + speedDifference;
		rightSpeedPID.setpoint = speedBaseOutput - speedDifference;

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

	disableForwardPID();
	disableDirectionPID();
	Brake();

	forwardPID._sumError = 0;
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
	while(1)
	{
		float data[] = { leftSpeed, rightSpeed, leftPWM, rightPWM };
		LogJustFloat(data, 4);
		SleepMillisecond(10);
	}
}

void KeepAngle(void)
{
	enableDirectionPID();
	while (1)
	{
		leftSpeedPID.setpoint = speedBaseOutput + speedDifference;
		rightSpeedPID.setpoint = speedBaseOutput - speedDifference;

		float data[] = { yaw, anglePID.setpoint, GetYawVelocity(), speedDifference, leftPWM, rightPWM };
		LogJustFloat(data, 6);

		SleepMillisecond(10);
	}
	disableDirectionPID();
	anglePID._sumError = 0;
	Brake();
}

void KeepDistance(uint16_t Distance)
{
	forwardPID.setpoint = Distance;
	enableDirectionPID();
	enableForwardPID();

	while(1)
	{
		leftSpeedPID.setpoint = speedBaseOutput + speedDifference;
		rightSpeedPID.setpoint = speedBaseOutput - speedDifference;
		float data[] = { distance, yaw, forwardPID.setpoint, speedBaseOutput, speedDifference };
		LogJustFloat(data, 5);
		SleepMillisecond(10);
	}

	disableForwardPID();
	disableDirectionPID();
	forwardPID._sumError = 0;
	Brake();
}

#endif

/**
 * @group: PID Calculate Handler
 */
__attribute__((always_inline)) inline void SpeedPIDCalculateHandler(void)
{
	leftSpeed  = GetLeftEncoderSpeed();
	rightSpeed = GetRightEncoderSpeed();

	if(getSpeedPID())
	{
		leftPWM = PosPID_Calc(&leftSpeedPID, leftSpeed);
		rightPWM = PosPID_Calc(&rightSpeedPID, rightSpeed);

		SetLeftMotorPWM(leftPWM);
		SetRightMotorPWM(rightPWM);
	}
}

__attribute__((always_inline)) inline void DirectionPIDCalculateHandler(void)
{
	if(getDirectionPIDStatus())
	{
		yaw = GetYawValue();
		speedDifference = PosPID_CalcWithCustErrAndDiff(&AnglePID, calculateYawError(yaw, anglePID.setpoint), GetYawVelocity());
	}
}

__attribute__((always_inline)) inline void ForwardPIDCalculateHandler(void)
{
	if(getForwardPIDStatus())
	{
		distance = GetTOF_Distance();
		speedBaseOutput = PosPID_Calc(&ForwardPID, distance);
	}
}
