/*
 * Servo.c
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#include "Servo.h"
#include "ServoPorts.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_tim.h"
#endif

/**
 * @brief: Control servos movement to the calibration point to facilitate mechanical calibration.
 */
void CalibrationAllServo(void)
{
	SetServo0Time(50);
	SetServo1Time(50);
	SetServo2Time(250);
	SetServo3Time(250);
	SetServo4Time(50);
	SetServo5Time(250);
}
