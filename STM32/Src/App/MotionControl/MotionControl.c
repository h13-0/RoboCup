/*
 * MotionControl.c
 *
 *  Created on: 2021��6��8��
 *      Author: h13
 */
#include "MotionControl.h"

#include <stdlib.h>

#include "Drivers.h"

#include "PIDManagement.h"
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

static const float angleAccurary = 2.0;
static const uint16_t forwardAccuracy = 20;
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
	uint8_t forwardPID_Status : 1;
};

static struct motionControlStatusStructure status;

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
 * @brief: Init Motion Control.
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
	anglePID.differention = 200.0;
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
	anglePID.differention = 417.0;
	anglePID.setpoint = 0.0;
	anglePID.maxAbsOutput = GetMaxValueOfPWM() * 0.4;

	//Extend functions config.
	anglePID.configs.autoResetIntegration = enable;
	anglePID.configs.limitIntegration = enable;
	anglePID.maximumAbsValueOfIntegrationOutput = 150.0;


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
 * @brief: Turn To ..
 * @param: Direction you want to turn to.
 */
void TurnTo(direction_t Direction)
{
	switch(Direction)
	{
	case Left:
		anglePID.setpoint += 90;
		break;

	case Right:
		anglePID.setpoint -= 90;
		break;

	case BackWard:

		break;

	default:
		break;
	}

	//Keep Angle
	enableDirectionPID();

	uint32_t startTime = GetMS();
	while(1)
	{
		SetLeftMotorPWM(pwmBaseOutput - pwmDifference);
		SetRightMotorPWM(pwmBaseOutput + pwmDifference);

		if((uint16_t)abs(yaw - anglePID.setpoint) > angleAccurary)
		{
			startTime = GetMS();
		}

		if(GetMS() - startTime > stableTimeLimit)
		{
			break;
		}
	}

	disableDirectionPID();
	Brake();

	anglePID._sumError = 0;
}

void StraightUntill(uint16_t Distance)
{
	//Keep Angle
	enableDirectionPID();

	forwardPID.setpoint = Distance;
	enableForwardPID();

	uint32_t startTime = GetMS();
	while (1)
	{
		SetLeftMotorPWM(pwmBaseOutput - pwmDifference);
		SetRightMotorPWM(pwmBaseOutput + pwmDifference);

		if((uint16_t)abs(distance - forwardPID.setpoint) > forwardAccuracy)
		{
			startTime = GetMS();
		}

		if(GetMS() - startTime > stableTimeLimit)
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
 */
void KeepAngle(uint16_t Angle)
{
	anglePID.setpoint = Angle;
	enableDirectionPID();
	while (1)
	{
		SetLeftMotorPWM(pwmBaseOutput - pwmDifference);
		SetRightMotorPWM(pwmBaseOutput + pwmDifference);

		float data[] = { yaw, anglePID.setpoint, pwmDifference };
		LogJustFloat(data, 3);
	}
	disableDirectionPID();
}

void KeepDistance(uint16_t Distance)
{
	forwardPID.setpoint = Distance;
	enableDirectionPID();
	enableForwardPID();

	while(1)
	{
		SetLeftMotorPWM(pwmBaseOutput - pwmDifference);
		SetRightMotorPWM(pwmBaseOutput + pwmDifference);
		float data[] = { distance, yaw, forwardPID.setpoint, pwmBaseOutput, pwmDifference };
		LogJustFloat(data, 5);
	}

	disableForwardPID();
	disableDirectionPID();
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
		float tempDifference = PosPID_Calc(&anglePID, yaw);
		pwmDifference = tempDifference;
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
