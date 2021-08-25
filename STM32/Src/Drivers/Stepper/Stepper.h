/*
 * Stepper.h
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */

#ifndef DRIVERS_STEPPER_STEPPER_H_
#define DRIVERS_STEPPER_STEPPER_H_

#include "GPIO.h"

typedef struct
{
	GPIO_t StepperIO;
	GPIO_t DirIO;

} Stepper_t;

void SteperTimerHandler(void);

#endif /* DRIVERS_STEPPER_STEPPER_H_ */
