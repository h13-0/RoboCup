/*
 * Motor.c
 *
 *  Created on: 2021Äê6ÔÂ3ÈÕ
 *      Author: h13
 */

#include "Motor.h"
#include "RobotConfigs.h"
#include "MotorPorts.h"

//Maximum PWM output value(Value of AutoReload Register + 1)
#define MAXPWMVALUE (1000)

/**
 * @brief:  Get the max value of pwm.
 * @return: MAXPWMVALUE - deadZone.
 */
__attribute__((always_inline)) inline uint16_t GetMaxValueOfPWM(void)
{
	return MAXPWMVALUE - ForwardDeadZone;
}

/**
 * @brief: Set the left motor PWM.
 * @param: PWM Value, Range: -1000 ~ 1000
 */
void SetLeftMotorPWM(int16_t pwmValue) {
	if (pwmValue > 0) {
		if(pwmValue < MAXPWMVALUE - ForwardDeadZone)
		{
			setChannel1Value(pwmValue + ForwardDeadZone);
			setChannel2Value(0);
		}else {
			setChannel1Value(MAXPWMVALUE);
			setChannel2Value(0);
		}
	} else if(pwmValue == 0) {
			setChannel1Value(0);
			setChannel2Value(0);
	} else {
		if(pwmValue > BackwardDeadZone - MAXPWMVALUE)
		{
			setChannel1Value(0);
			setChannel2Value(- pwmValue + BackwardDeadZone);
		}else{
			setChannel1Value(0);
			setChannel2Value(MAXPWMVALUE);
		}
	}
}

/**
 * @brief: Set the Right motor PWM.
 * @param: PWM Value, Range: -1000 ~ 1000
 */
void SetRightMotorPWM(int16_t pwmValue)
{
	if (pwmValue > 0) {
		if(pwmValue < MAXPWMVALUE - ForwardDeadZone)
		{
			setChannel3Value(pwmValue + ForwardDeadZone);
			setChannel4Value(0);
		}else {
			setChannel3Value(MAXPWMVALUE);
			setChannel4Value(0);
		}
	} else if(pwmValue == 0) {
			setChannel3Value(0);
			setChannel4Value(0);
	} else {
		if(pwmValue > BackwardDeadZone - MAXPWMVALUE)
		{
			setChannel3Value(0);
			setChannel4Value(- pwmValue + BackwardDeadZone);
		}else{
			setChannel3Value(0);
			setChannel4Value(MAXPWMVALUE);
		}
	}
}

/**
 * @brief: Brake(Apply electromagnetic damped brakes)
 */
void Brake(void)
{
	setChannel1Value(MAXPWMVALUE);
	setChannel2Value(MAXPWMVALUE);
	setChannel3Value(MAXPWMVALUE);
	setChannel4Value(MAXPWMVALUE);
}
