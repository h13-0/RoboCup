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

/**
 * @group: Configs
 * @param:
 * 		angleAccuracy:   Angle accuracy limit in angle.
 * 		forwardAccuracy: Forward accuracy limit in millimeter.
 * 		stableTimeLimit: Stable Time Limit in milliseconds.
 */
#define Smooth 0
#define Normal 1
#define Rough  2

#define Roughness Normal

#define stableTimeLimit 500
#if(stableTimeLimit > 12750)
#error "stableTimeLimit must less than 12750"
#endif


#ifdef DEBUG
//Share PID Structure with PIDAdjust.c
PositionPID_t AnglePID;
PositionPID_t ForwardPID;
#define anglePID   AnglePID
#define forwardPID ForwardPID

#else
static PositionPID_t anglePID;
static PositionPID_t forwardPID;
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
	uint8_t directionPID_Status : 1;
	uint8_t forwardPID_Status   : 1;
};

static struct motionControlStatusStructure status = { 0 };

//Status of direction PID.
#define enableDirectionPID()          status.directionPID_Status = 1
#define disableDirectionPID()         status.directionPID_Status = 0
#define getDirectionPIDStatus()       status.directionPID_Status
//Status of forward PID.
#define enableForwardPID()            status.forwardPID_Status = 1
#define disableForwardPID()           status.forwardPID_Status = 0
#define getForwardPIDStatus()         status.forwardPID_Status


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
 *
 */
static float pwmDifference = 0;
static float pwmBaseOutput = 0;


/**
 * @brief: Init motion control.
 * @note:
 * 		Before you can use this, you need to initialize Motor and MPU manually ahead of time.
 * 		It will Init Direction PID, turn to zero and brake.
 */
void MotionControlInit(void)
{
#if(Roughness == Smooth)
	//angle PID configs.
	anglePID.proportion = 5.0;
	anglePID.integration = 0.95;
	anglePID.differention = 2.0;
	anglePID.setpoint = 0.0;
	anglePID.maxAbsOutput = GetMaxValueOfPWM() * 0.3;

	//Extend functions config.
	anglePID.configs.autoResetIntegration = enable;
	anglePID.configs.limitIntegration = enable;
	anglePID.maximumAbsValueOfIntegrationOutput = 75.0;


	forwardPID.proportion = -1.0;
	forwardPID.integration = 0.0;
	forwardPID.differention = 0.0;
	forwardPID.setpoint = 0.0;
	forwardPID.maxAbsOutput = GetMaxValueOfPWM() * 0.5;

	//Extend functions config.
	forwardPID.configs.autoResetIntegration = enable;
	forwardPID.configs.limitIntegration = enable;
	forwardPID.maximumAbsValueOfIntegrationOutput = 200.0;
#elif(Roughness == Normal)
	//angle PID configs.
	anglePID.proportion = 7.5;
	anglePID.integration = 1.8;
	anglePID.differention = 4.2;
	anglePID.setpoint = 0.0;
	anglePID.maxAbsOutput = GetMaxValueOfPWM() * 0.5;

	//Extend functions config.
	anglePID.configs.autoResetIntegration = enable;
	anglePID.configs.limitIntegration = enable;
	anglePID.maximumAbsValueOfIntegrationOutput = 300.0;


	forwardPID.proportion = -1.0;
	forwardPID.integration = 0.0;
	forwardPID.differention = 0.0;
	forwardPID.setpoint = 0.0;
	forwardPID.maxAbsOutput = GetMaxValueOfPWM() * 0.7;

	//Extend functions config.
	forwardPID.configs.autoResetIntegration = disable;
	forwardPID.configs.limitIntegration = enable;
	forwardPID.maximumAbsValueOfIntegrationOutput = forwardPID.maxAbsOutput;
#elif(Roughness == Rough)
	//angle PID configs.
	anglePID.proportion = 7.5;
	anglePID.integration = 1.8;
	anglePID.differention = 4.2;
	anglePID.setpoint = 0.0;
	anglePID.maxAbsOutput = GetMaxValueOfPWM() * 0.5;

	//Extend functions config.
	anglePID.configs.autoResetIntegration = enable;
	anglePID.configs.limitIntegration = enable;
	anglePID.maximumAbsValueOfIntegrationOutput = 300.0;


	forwardPID.proportion = -1.0;
	forwardPID.integration = 0.0;
	forwardPID.differention = 0.0;
	forwardPID.setpoint = 0.0;
	forwardPID.maxAbsOutput = GetMaxValueOfPWM() * 0.5;

	//Extend functions config.
	forwardPID.configs.autoResetIntegration = disable;
	forwardPID.configs.limitIntegration = enable;
	forwardPID.maximumAbsValueOfIntegrationOutput = 200.0;
#endif
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

	//Keep Angle
	anglePID._sumError = 0;
	enableDirectionPID();

	uint32_t startTime = GetCurrentTimeMillisecond();
	uint32_t startStableTime = GetCurrentTimeMillisecond();

	while(1)
	{
		SetLeftMotorPWM(pwmBaseOutput + pwmDifference);
		SetRightMotorPWM(pwmBaseOutput - pwmDifference);

		if(fabs(calculateYawError(yaw, anglePID.setpoint) > AngleAccurary))
		{
			startStableTime = GetCurrentTimeMillisecond();
		}

		if(GetCurrentTimeMillisecond() - startStableTime > stableTimeLimit)
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
	while (1)
	{
		SetLeftMotorPWM(pwmBaseOutput + pwmDifference);
		SetRightMotorPWM(pwmBaseOutput - pwmDifference);

		if((uint16_t)fabs(distance - forwardPID.setpoint) > ForwardAccuracy)
		{
			startTime = GetCurrentTimeMillisecond();
		}

		if(GetCurrentTimeMillisecond() - startTime > stableTimeLimit)
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
void KeepAngle(void)
{
	enableDirectionPID();
	while (1)
	{
		SetLeftMotorPWM(pwmBaseOutput + pwmDifference);
		SetRightMotorPWM(pwmBaseOutput - pwmDifference);

		float data[] = { yaw, anglePID.setpoint, GetYawVelocity(), pwmDifference };
		LogJustFloat(data, 4);

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
		SetLeftMotorPWM(pwmBaseOutput + pwmDifference);
		SetRightMotorPWM(pwmBaseOutput - pwmDifference);
		float data[] = { distance, yaw, forwardPID.setpoint, pwmBaseOutput, pwmDifference };
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
//WARNING: **pay attention to the zero point problem**
__attribute__((always_inline)) inline void DirectionPIDCalculateHandler(void)
{
	if (getDirectionPIDStatus())
	{
		yaw = GetYawValue();
		pwmDifference = PosPID_CalcWithCustErrAndDiff(&AnglePID, calculateYawError(yaw, anglePID.setpoint), GetYawVelocity());
	}
}

__attribute__((always_inline)) inline void ForwardPIDCalculateHandler(void)
{
	if (getForwardPIDStatus())
	{
		distance = GetTOF_Distance();
		pwmBaseOutput = PosPID_Calc(&ForwardPID, distance);
	}
}
