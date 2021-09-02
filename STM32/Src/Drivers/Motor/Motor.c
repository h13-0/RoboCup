/*
 * Motor.c
 *
 *  Created on: 2021Äê6ÔÂ3ÈÕ
 *      Author: h13
 */
#include "Motor.h"
#include "RobotConfigs.h"
#include <assert.h>
#include "Ports.h"

#define PositiveZero (0.001)
#define NegativeZero (-0.001)

static PWM_t  leftPWM  = LeftMotorPWM;
static PWM_t  rightPWM = RightMotorPWM;
static GPIO_t leftIO   = LeftMotorIO;
static GPIO_t rightIO  = RightMotorIO;

/**
 * @brief: Initialize the H-bridge module and PWM peripherals.
 */
void MotorInit()
{
	assert(leftPWM.Frequency == rightPWM.Frequency);
	PWM_Init(&leftPWM);
	PWM_Init(&rightPWM);
}

/**
 * @brief:  Get the max value of pwm.
 * @return: 1.0 - deadZone.
 */
__attribute__((always_inline)) inline float GetMaxValueOfPWM(void)
{
	return 1.0 - ForwardDeadZone;
}

/**
 * @brief: Set the left motor PWM.
 * @param: PWM Value, Range: [-1.0, 1.0]
 */
void SetLeftMotorPWM(float PWM_DutyRatio)
{
#if(!ReverseLeftRight)
#if(ReverseChannel1)
	PWM_DutyRatio = - PWM_DutyRatio;
#endif
	if(PWM_DutyRatio > PositiveZero)
	{
		PullDownGPIO(&leftIO);
		float ratio = PWM_DutyRatio + ForwardDeadZone;
		if(ratio < 1.0)
		{
			SetPWM_DutyRatio(&leftPWM, ratio);
		} else {
			PullDownGPIO(&leftIO);
			SetPWM_DutyRatio(&leftPWM, 1.0);
		}

	} else if(PWM_DutyRatio < NegativeZero)
	{
		PullUpGPIO(&leftIO);
		float ratio = 1.0 + PWM_DutyRatio - BackwardDeadZone;
		if(ratio > 0)
		{
			SetPWM_DutyRatio(&leftPWM, ratio);
		} else {
			SetPWM_DutyRatio(&leftPWM, 0);
		}
	} else {
		PullDownGPIO(&leftIO);
		SetPWM_DutyRatio(&leftPWM, 0);
	}
#else
#if(ReverseChannel2)
	PWM_DutyRatio = - PWM_DutyRatio;
#endif
	if(PWM_DutyRatio > PositiveZero)
	{
		PullDownGPIO(&rightIO);
		float ratio = PWM_DutyRatio + ForwardDeadZone;
		if(ratio < 1.0)
		{
			SetPWM_DutyRatio(&rightPWM, ratio);
		} else {
			SetPWM_DutyRatio(&rightPWM, 1.0);
		}

	} else if(PWM_DutyRatio < NegativeZero)
	{
		PullUpGPIO(&rightIO);
		float ratio = 1.0 + PWM_DutyRatio - BackwardDeadZone;
		if(ratio > 0)
		{
			SetPWM_DutyRatio(&rightPWM, ratio);
		} else {
			SetPWM_DutyRatio(&rightPWM, 0);
		}
	} else {
		PullDownGPIO(&rightIO);
		SetPWM_DutyRatio(&rightPWM, 0);
	}
#endif
}

/**
 * @brief: Set the Right motor PWM.
 * @param: PWM Value, Range: [-1.0, 1.0]
 */
void SetRightMotorPWM(float PWM_DutyRatio)
{
#if(!ReverseLeftRight)
#if(ReverseChannel2)
	PWM_DutyRatio = - PWM_DutyRatio;
#endif
	if(PWM_DutyRatio > PositiveZero)
	{
		PullDownGPIO(&rightIO);
		float ratio = PWM_DutyRatio + ForwardDeadZone;
		if(ratio < 1.0)
		{
			SetPWM_DutyRatio(&rightPWM, ratio);
		} else {
			SetPWM_DutyRatio(&rightPWM, 1.0);
		}

	} else if(PWM_DutyRatio < NegativeZero)
	{
		PullUpGPIO(&rightIO);
		float ratio = 1.0 + PWM_DutyRatio - BackwardDeadZone;
		if(ratio > 0)
		{
			SetPWM_DutyRatio(&rightPWM, ratio);
		} else {
			SetPWM_DutyRatio(&rightPWM, 0);
		}
	} else {
		PullDownGPIO(&rightIO);
		SetPWM_DutyRatio(&rightPWM, 0);
	}
#else
#if(ReverseChannel1)
	PWM_DutyRatio = - PWM_DutyRatio;
#endif
	if(PWM_DutyRatio > PositiveZero)
	{
		PullDownGPIO(&leftIO);
		float ratio = PWM_DutyRatio + ForwardDeadZone;
		if(ratio < 1.0)
		{
			SetPWM_DutyRatio(&leftPWM, ratio);
		} else {
			PullDownGPIO(&leftIO);
			SetPWM_DutyRatio(&leftPWM, 1.0);
		}

	} else if(PWM_DutyRatio < NegativeZero)
	{
		PullUpGPIO(&leftIO);
		float ratio = 1.0 + PWM_DutyRatio - BackwardDeadZone;
		if(ratio > 0)
		{
			SetPWM_DutyRatio(&leftPWM, ratio);
		} else {
			SetPWM_DutyRatio(&leftPWM, 0);
		}
	} else {
		PullDownGPIO(&leftIO);
		SetPWM_DutyRatio(&leftPWM, 0);
	}
#endif
}

/**
 * @brief: Brake(Apply electromagnetic damped brakes)
 */
void Brake(void)
{
	PullUpGPIO(&leftIO);
	SetPWM_DutyRatio(&leftPWM, 1.0);
	PullUpGPIO(&rightIO);
	SetPWM_DutyRatio(&rightPWM, 1.0);
}
