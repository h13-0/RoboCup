/*
 * Motor.c
 *
 *  Created on: 2021Äê6ÔÂ3ÈÕ
 *      Author: h13
 */

#include "Motor.h"
#include "RobotConfigs.h"
#include "Ports.h"

//Maximum PWM output value(Value of AutoReload Register + 1)
#define MAXPWMVALUE (1000)

static PWM_t  leftPWM  = LeftMotorPWM;
static PWM_t  rightPWM = RightMotorPWM;
static GPIO_t leftIO   = LeftMotorIO;
static GPIO_t rightIO  = RightMotorIO;

/**
 * @brief:  Get the max value of pwm.
 * @return: MAXPWMVALUE - deadZone.
 */
__attribute__((always_inline)) inline uint16_t GetMaxValueOfPWM(void)
{
	return MAXPWMVALUE - ForwardDeadZone;
}

void MotorInit()
{
	PWM_Init(&leftPWM, LeftMotorPWM_Frequence, LeftMotorPWM_ReloadValue);
	PWM_Init(&rightPWM, RightMotorPWM_Frequence, RightMotorPWM_ReloadValue);
}

/**
 * @brief: Set the left motor PWM.
 * @param: PWM Value, Range: -1000 ~ 1000
 */
void SetLeftMotorPWM(int16_t PWM_Value)
{
#if(!ReverseLeftRight)
#if(ReverseChannel1)
	PWM_Value = - PWM_Value;
#endif
	if(PWM_Value > 0)
	{
		if(PWM_Value < MAXPWMVALUE - ForwardDeadZone)
		{
			PullDownGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, PWM_Value + ForwardDeadZone);
		} else {
			PullDownGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, MAXPWMVALUE);
		}

	} else if(PWM_Value < 0)
	{
		if(PWM_Value > BackwardDeadZone - MAXPWMVALUE)
		{
			PullUpGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, MAXPWMVALUE + PWM_Value - BackwardDeadZone);
		} else {
			PullUpGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, 0);
		}
	} else {
		PullDownGPIO(&leftIO);
		SetPWM_HighLevelCompareValue(&leftPWM, 0);
	}
#else
#if(ReverseChannel2)
	PWM_Value = - PWM_Value;
#endif
	if(PWM_Value > 0)
	{
		if(PWM_Value < MAXPWMVALUE - ForwardDeadZone)
		{
			PullDownGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, PWM_Value + ForwardDeadZone);
		} else {
			PullDownGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, MAXPWMVALUE);
		}

	} else if(PWM_Value < 0)
	{
		if(PWM_Value > BackwardDeadZone - MAXPWMVALUE)
		{
			PullUpGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, MAXPWMVALUE + PWM_Value - BackwardDeadZone);
		} else {
			PullUpGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, 0);
		}
	} else {
		PullDownGPIO(&rightIO);
		SetPWM_HighLevelCompareValue(&rightPWM, 0);
	}
#endif
}

/**
 * @brief: Set the Right motor PWM.
 * @param: PWM Value, Range: -1000 ~ 1000
 */
void SetRightMotorPWM(int16_t PWM_Value)
{
#if(!ReverseLeftRight)
#if(ReverseChannel2)
	PWM_Value = - PWM_Value;
#endif
	if(PWM_Value > 0)
	{
		if(PWM_Value < MAXPWMVALUE - ForwardDeadZone)
		{
			PullDownGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, PWM_Value + ForwardDeadZone);
		} else {
			PullDownGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, MAXPWMVALUE);
		}

	} else if(PWM_Value < 0)
	{
		if(PWM_Value > BackwardDeadZone - MAXPWMVALUE)
		{
			PullUpGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, MAXPWMVALUE + PWM_Value - BackwardDeadZone);
		} else {
			PullUpGPIO(&rightIO);
			SetPWM_HighLevelCompareValue(&rightPWM, 0);
		}
	} else {
		PullDownGPIO(&rightIO);
		SetPWM_HighLevelCompareValue(&rightPWM, 0);
	}
#else
#if(ReverseChannel1)
	PWM_Value = - PWM_Value;
#endif
	if(PWM_Value > 0)
	{
		if(PWM_Value < MAXPWMVALUE - ForwardDeadZone)
		{
			PullDownGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, PWM_Value + ForwardDeadZone);
		} else {
			PullDownGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, MAXPWMVALUE);
		}

	} else if(PWM_Value < 0)
	{
		if(PWM_Value > BackwardDeadZone - MAXPWMVALUE)
		{
			PullUpGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, MAXPWMVALUE + PWM_Value - BackwardDeadZone);
		} else {
			PullUpGPIO(&leftIO);
			SetPWM_HighLevelCompareValue(&leftPWM, 0);
		}
	} else {
		PullDownGPIO(&leftIO);
		SetPWM_HighLevelCompareValue(&leftPWM, 0);
	}
#endif
}

/**
 * @brief: Brake(Apply electromagnetic damped brakes)
 */
void Brake(void)
{
	PullUpGPIO(&leftIO);
	SetPWM_HighLevelCompareValue(&leftPWM, MAXPWMVALUE);
	PullUpGPIO(&rightIO);
	SetPWM_HighLevelCompareValue(&rightPWM, MAXPWMVALUE);
}
