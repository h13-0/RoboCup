/*
 * Motor.h
 *
 *  Created on: 2021Äê6ÔÂ3ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_MOTOR_MOTOR_H_
#define PLATFORM_MOTOR_MOTOR_H_

#include <stdint.h>

/**
 * @brief: Initialize the H-bridge module and PWM peripherals.
 */
void MotorInit(void);

/**
 * @brief:  Get the max value of pwm.
 * @return: 1.0 - ForwardDeadZone.
 */
float GetMaxValueOfPWM(void);

/**
 * @brief: Set the left motor PWM.
 * @param: PWM Value, Range: [-1.0, 1.0]
 */
void SetLeftMotorPWM(float PWM_DutyRatio);

/**
 * @brief: Set the Right motor PWM.
 * @param: PWM Value, Range: [-1.0, 1.0]
 */
void SetRightMotorPWM(float PWM_DutyRatio);

/**
 * @brief: Brake(Apply electromagnetic damped brakes)
 */
void Brake(void);

#endif /* PLATFORM_MOTOR_MOTOR_H_ */
