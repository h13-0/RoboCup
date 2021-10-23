/*
 * Stepper.h
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */

#ifndef DRIVERS_STEPPER_STEPPER_H_
#define DRIVERS_STEPPER_STEPPER_H_

#include "GPIO.h"
#include "RobotConfigs.h"

#if(ArmType == LiftingPlatform)

#include <stdint.h>

void SteperTimerHandler(void);

typedef enum
{
	StepperOK = 0,
	StepperBufferIsFull = 1,
} StepperError_t;

typedef struct
{
	GPIO_t StepperIO;
	GPIO_t DirectionIO;
	int32_t CurrentSteps;
	int32_t TargetSteps;
} Stepper_t;

StepperError_t StepperInit(Stepper_t *Stepper);

typedef enum
{
	Forward,
	Backward,
} Direction_t;

void SetStepperSteps(Stepper_t *Stepper, uint32_t TargetSteps);

void StepperForward(Stepper_t *Stepper, int32_t Steps);

void StepperBackward(Stepper_t *Stepper, int32_t Steps);

#endif /* ArmType == LiftingPlatform */

#endif /* DRIVERS_STEPPER_STEPPER_H_ */
