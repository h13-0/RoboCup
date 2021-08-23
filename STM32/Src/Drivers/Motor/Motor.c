/*
 * Motor.c
 *
 *  Created on: 2021Äê6ÔÂ3ÈÕ
 *      Author: h13
 */

#include "Motor.h"
#include "MotorPorts.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx.h"
#endif

//Maximum PWM output value(Value of AutoReload Register + 1)
#define MAXPWMVALUE (1000)

/**
 * @configs:
 *		deadZone:
 *			Loaded:  335
 */
static const uint16_t deadZone = 335;

/**
 * @brief:  Get the max value of pwm.
 * @return: MAXPWMVALUE - deadZone.
 */
__attribute__((always_inline)) inline uint16_t GetMaxValueOfPWM(void)
{
	return MAXPWMVALUE - deadZone;
}

/**
 * @brief: Set the left motor PWM.
 * @param: PWM Value, Range: -1000 ~ 1000
 */
void SetLeftMotorPWM(int16_t pwmValue) {
	if (pwmValue > 0) {
		if(pwmValue < GetMaxValueOfPWM())
		{
			setChannel1Value(pwmValue + deadZone);
			setChannel2Value(0);
		}else {
			setChannel1Value(MAXPWMVALUE);
			setChannel2Value(0);
		}
	} else if(pwmValue == 0) {
			setChannel1Value(0);
			setChannel2Value(0);
	} else {
		if(pwmValue > (- GetMaxValueOfPWM()))
		{
			setChannel1Value(0);
			setChannel2Value(- pwmValue + deadZone);
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
		if(pwmValue < GetMaxValueOfPWM())
		{
			setChannel3Value(pwmValue + deadZone);
			setChannel4Value(0);
		}else {
			setChannel3Value(MAXPWMVALUE);
			setChannel4Value(0);
		}
	} else if(pwmValue == 0) {
			setChannel3Value(0);
			setChannel4Value(0);
	} else {
		if(pwmValue > (- GetMaxValueOfPWM()))
		{
			setChannel3Value(0);
			setChannel4Value(- pwmValue + deadZone);
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
