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

#if(!ReverseLeftRight)

#if(!ReverseChannel1)
static PWM_t channelD0 = PWM_ChannelD0;
static PWM_t channelD1 = PWM_ChannelD1;
#else
static PWM_t channelD0 = PWM_ChannelD1;
static PWM_t channelD1 = PWM_ChannelD0;
#endif

#if(!ReverseChannel2)
static PWM_t channelD2 = PWM_ChannelD2;
static PWM_t channelD3 = PWM_ChannelD3;
#else
static PWM_t channelD2 = PWM_ChannelD3;
static PWM_t channelD3 = PWM_ChannelD2;
#endif

#else

#if(!ReverseChannel1)
static PWM_t channelD0 = PWM_ChannelD2;
static PWM_t channelD1 = PWM_ChannelD3;
#else
static PWM_t channelD0 = PWM_ChannelD3;
static PWM_t channelD1 = PWM_ChannelD2;
#endif

#if(!ReverseChannel2)
static PWM_t channelD2 = PWM_ChannelD0;
static PWM_t channelD3 = PWM_ChannelD1;
#else
static PWM_t channelD2 = PWM_ChannelD1;
static PWM_t channelD3 = PWM_ChannelD0;
#endif

#endif

/**
 * @brief: Initialize the H-bridge module and PWM peripherals.
 */
void MotorInit()
{
	assert((channelD0.Frequency == channelD1.Frequency) && (channelD2.Frequency == channelD3.Frequency));
	assert((channelD0.ReloadValue == channelD1.ReloadValue) && (channelD2.ReloadValue == channelD3.ReloadValue));

	PWM_Init(&channelD0);
	PWM_Init(&channelD1);
	PWM_Init(&channelD2);
	PWM_Init(&channelD3);
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
	if(PWM_DutyRatio > PositiveZero)
	{
		PWM_DutyRatio += ForwardDeadZone;
		if(PWM_DutyRatio < 1.0)
		{
			SetPWM_DutyRatio(&channelD0, PWM_DutyRatio);
			SetPWM_DutyRatio(&channelD1, 0);
		} else {
			SetPWM_DutyRatio(&channelD0, 1);
			SetPWM_DutyRatio(&channelD1, 0);
		}
	} else if(PWM_DutyRatio < NegativeZero)
	{
		PWM_DutyRatio = - PWM_DutyRatio + BackwardDeadZone;
		if(PWM_DutyRatio > - 1.0)
		{
			SetPWM_DutyRatio(&channelD0, 0);
			SetPWM_DutyRatio(&channelD1, PWM_DutyRatio);
		} else {
			SetPWM_DutyRatio(&channelD0, 0);
			SetPWM_DutyRatio(&channelD1, 1);
		}
	} else {
		SetPWM_DutyRatio(&channelD0, 0);
		SetPWM_DutyRatio(&channelD1, 0);
	}
}

/**
 * @brief: Set the Right motor PWM.
 * @param: PWM Value, Range: [-1.0, 1.0]
 */
void SetRightMotorPWM(float PWM_DutyRatio)
{
	if(PWM_DutyRatio > PositiveZero)
	{
		PWM_DutyRatio += ForwardDeadZone;
		if(PWM_DutyRatio < 1.0)
		{
			SetPWM_DutyRatio(&channelD2, PWM_DutyRatio);
			SetPWM_DutyRatio(&channelD3, 0);
		} else {
			SetPWM_DutyRatio(&channelD2, 1);
			SetPWM_DutyRatio(&channelD3, 0);
		}
	} else if(PWM_DutyRatio < NegativeZero)
	{
		PWM_DutyRatio = - PWM_DutyRatio + BackwardDeadZone;
		if(PWM_DutyRatio > - 1.0)
		{
			SetPWM_DutyRatio(&channelD2, 0);
			SetPWM_DutyRatio(&channelD3, PWM_DutyRatio);
		} else {
			SetPWM_DutyRatio(&channelD2, 0);
			SetPWM_DutyRatio(&channelD3, 1);
		}
	} else {
		SetPWM_DutyRatio(&channelD2, 0);
		SetPWM_DutyRatio(&channelD3, 0);
	}
}

/**
 * @brief: Brake(Apply electromagnetic damped brakes)
 */
void Brake(void)
{
	SetPWM_DutyRatio(&channelD0, 1);
	SetPWM_DutyRatio(&channelD1, 1);
	SetPWM_DutyRatio(&channelD2, 1);
	SetPWM_DutyRatio(&channelD3, 1);
}
