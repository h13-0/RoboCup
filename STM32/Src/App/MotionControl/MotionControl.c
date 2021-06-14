/*
 * MotionControl.c
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */
#include "MotionControl.h"

#include <math.h>

#include "PIDManagement.h"
#include "PID.h"
#include "ports.h"
#include "BitOperation.h"

/**
 * @group: Configs
 * @param:
 * 		angleAccuracy:   Angle accuracy limit in angle.
 * 		forwardAccuracy: Forward accuracy limit in millimeter.
 * 		stableTimeLimit: Stable Time Limit in milliseconds.
 */
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
 * 		Bit1: Last error status of direction PID (To clear the integral term).
 * 				positive -> 1.
 * 				negative -> 0.
 * 		Bit2: Status of forward PID.
 * 				Enable   -> 1.
 * 				Disable  -> 0.
 * 		Bit3: Last error status of forward PID (To clear the integral term).
 * 				Enable   -> 1.
 * 				Disable  -> 0.
 */
static uint8_t pidStatus = 0;
//Bit0: Status of direction PID.
#define enableDirectionPID()          Set8Bit0(pidStatus)
#define disableDirectionPID()         Reset8Bit0(pidStatus)
#define getDirectionPIDStatus()       Get8Bit0(pidStatus)
//Bit1: Last error status of direction PID (To clear the integral term).
#define setDirectionPIDLastStatus()   Set8Bit1(pidStatus)
#define resetDirectionPIDLastStatus() Reset8Bit1(pidStatus)
#define getDirectionPIDLastStatus()   Get8Bit1(pidStatus)
//Bit2: Status of forward PID.
#define enableForwardPID()            Set8Bit2(pidStatus)
#define disableForwardPID()           Reset8Bit2(pidStatus)
#define getForwardPIDStatus()         Get8Bit2(pidStatus)
//Bit3: Last error status of forward PID (To clear the integral term).
#define setForwardPIDLastStatus()     Set8Bit3(pidStatus)
#define resetForwardPIDLastStatus()   Reset8Bit3(pidStatus)
#define getForwardPIDLastStatus()     Get8Bit3(pidStatus)

/**
 * @group: Direction Variables.
 */
//
static float pitch = 0, roll = 0, yaw = 0;

static const float anglePIDSumErrorLimit = 500.0;

static float pwmDifference = 0;
static const float maxDifference = 500.0;

/**
 * @group: Forward Variables.
 */
static float distance = 0.0;

/**
 *
 */
static float pwmBaseOutput = 0;


/**
 * @brief: Init Motion Control.
 * @note:
 * 		Before you can use this, you need to initialize Motor and MPU manually ahead of time.
 * 		It will Init Direction PID, turn to zero and brake.
 */
void MotionControlInit(void)
{
	anglePID.proportion = 5.0;
	anglePID.integral = 0.75;
	anglePID.derivative = 200.0;
	anglePID.setpoint = 0.0;

	forwardPID.proportion = 0.0;
	forwardPID.integral = 0.0;
	forwardPID.derivative = 0.0;
	forwardPID.setpoint = 0.0;
}

/**
 * @brief: Turn To ..
 * @param: Direction you want to turn to.
 */
void TurnTo(direction_t Direction)
{
	switch(Direction)
	{

	}
}

void StraightUntill(uint16_t Distance)
{
	//Keep Angle
	enableDirectionPID();

#ifdef DEBUG
	//If proportion of forwardPID is set.
	if (forwardPID.proportion > 0.01)
	{
#endif
		//Full speed forward or backward before the control conditions are met.
		if (distance < (Distance - (GetMaxValueOfPWM() / forwardPID.proportion)))
		{
			pwmBaseOutput = GetMaxValueOfPWM() * 0.8;
			while (distance < (Distance - (GetMaxValueOfPWM() / forwardPID.proportion)));
		} else if (distance > (Distance + (GetMaxValueOfPWM() / forwardPID.proportion)))
		{
			pwmBaseOutput = -GetMaxValueOfPWM() * 0.8;
			while (distance > (Distance + (GetMaxValueOfPWM() / forwardPID.proportion)));
		}
#ifdef DEBUG
	}
#endif

	//Switch to PID Control when the distance is close enough.
	enableForwardPID();
	while (1)
	{
		static uint8_t stableTimes = 0;

		if(abs(distance - forwardPID.setpoint) < forwardAccuracy)
		{
			stableTimes++;
		}else
		{
			stableTimes = 0;
		}

		if(stableTimes > stableTimeLimit / 50)
		{
			break;
		}
		DelayMS(50);
	}

	disableForwardPID();
	disableDirectionPID();
	Brake();
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
		MPUGetData(&pitch, &roll, &yaw);
		SetLeftMotorPWM(pwmBaseOutput - pwmDifference);
		SetRightMotorPWM(pwmBaseOutput + pwmDifference);
		float data[] = { pitch, roll, yaw, anglePID.setpoint, pwmDifference };
		LogJustFloat(data, 5);
	}
	disableDirectionPID();
}

void KeepDistance(uint16_t Distance)
{
	enableDirectionPID();
	enableForwardPID();

	while(1)
	{

	}

	disableForwardPID();
	disableDirectionPID();
	Brake();
}

#endif



/**
 * @brief: Update Distance Handler.
 * @param: Distance in millimeter.
 */
__attribute__((always_inline)) inline void UpdateDistanceHandler(uint16_t Distance)
{
	distance = Distance;
}

/**
 * @group: PID Calculate Handler
 */
//WARNING: **pay attention to the zero point problem**
__attribute__((always_inline)) inline void DirectionPIDCalculateHandler(void)
{
	if (getDirectionPIDStatus())
	{
		if (getDirectionPIDLastStatus() && (yaw < anglePID.setpoint))
		{
			resetDirectionPIDLastStatus();
			anglePID._sum_error = 0;
		} else if ((!getDirectionPIDLastStatus()) && (yaw > anglePID.setpoint))
		{
			setDirectionPIDLastStatus();
			anglePID._sum_error = 0;
		}

		if(anglePID._sum_error > anglePIDSumErrorLimit)
		{
			anglePID._sum_error = anglePIDSumErrorLimit;
		}else if(anglePID._sum_error < -anglePIDSumErrorLimit)
		{
			anglePID._sum_error = -anglePIDSumErrorLimit;
		}

		float tempDifference = PosPIDCalc(&anglePID, yaw);
		if(tempDifference > maxDifference)
		{
			tempDifference = maxDifference;
		}else if(tempDifference < -maxDifference)
		{
			tempDifference = -maxDifference;
		}
		pwmDifference = tempDifference;
	}
}

__attribute__((always_inline)) inline void ForwardPIDCalculateHandler(void)
{
	if (getForwardPIDStatus())
	{
		if (getForwardPIDLastStatus() && (distance < forwardPID.setpoint))
		{
			resetForwardPIDLastStatus();
			forwardPID._sum_error = 0;
		} else if ((!getForwardPIDLastStatus()) && (distance > forwardPID.setpoint))
		{
			setForwardPIDLastStatus();
			forwardPID._sum_error = 0;
		}
		pwmBaseOutput = PosPIDCalc(&ForwardPID, distance);
	}
}
