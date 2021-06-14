/*
 * Motor.h
 *
 *  Created on: 2021Äê6ÔÂ3ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_MOTOR_MOTOR_H_
#define PLATFORM_MOTOR_MOTOR_H_

#include <stdio.h>

/**
 * @brief:  Get the max value of pwm.
 * @return: MAXPWMVALUE - deadZone.
 */
uint16_t GetMaxValueOfPWM(void);

/**
 * @brief: Set the left motor PWM.
 * @param: PWM Value, Range: -1000 ~ 1000
 */
void SetLeftMotorPWM(int16_t pwmValue);

/**
 * @brief: Set the Right motor PWM.
 * @param: PWM Value, Range: -1000 ~ 1000
 */
void SetRightMotorPWM(int16_t pwmValue);

/**
 * @brief: Brake(Apply electromagnetic damped brakes)
 */
void Brake(void);

#endif /* PLATFORM_MOTOR_MOTOR_H_ */
