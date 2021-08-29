/*
 * Stepper.c
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */

#include "Stepper.h"
#include "RobotConfigs.h"

#if(ArmType == LiftingPlatform)

#include <stddef.h>

#include "GPIO.h"

static Stepper_t *steppers[StepperNumbers] = { NULL };
static uint8_t currentStepperNumbers = 0;

void SteperTimerHandler(void)
{
	static uint8_t oddEvenSteps = 0;

	if(oddEvenSteps)
	{
		oddEvenSteps = 0;
		for(uint8_t i = 0; i < currentStepperNumbers; i++)
		{
			if(steppers[i] -> CurrentSteps < steppers[i] -> TargetSteps)
			{
				PullUpGPIO(&(steppers[i] -> DirectionIO));
				PullUpGPIO(&(steppers[i] -> StepperIO));
				steppers[i] -> CurrentSteps ++;
			} else if(steppers[i] -> CurrentSteps > steppers[i] -> TargetSteps) {

				PullDownGPIO(&(steppers[i] -> DirectionIO));
				PullUpGPIO(&(steppers[i] -> StepperIO));
				steppers[i] -> CurrentSteps --;
			}
		}
	} else {
		oddEvenSteps = 1;
		for(uint8_t i = 0; i < currentStepperNumbers; i++)
		{
			if(steppers[i] -> CurrentSteps < steppers[i] -> TargetSteps)
			{
				PullUpGPIO(&(steppers[i] -> DirectionIO));
				PullDownGPIO(&(steppers[i] -> StepperIO));
			} else if(steppers[i] -> CurrentSteps > steppers[i] -> TargetSteps) {
				PullDownGPIO(&(steppers[i] -> DirectionIO));
				PullDownGPIO(&(steppers[i] -> StepperIO));
			}
		}
	}
}

StepperError_t StepperInit(Stepper_t *Stepper)
{
	if(currentStepperNumbers < StepperNumbers - 1)
	{
		steppers[currentStepperNumbers] = Stepper;
		currentStepperNumbers ++;
		return StepperOK;
	} else {
		return StepperBufferIsFull;
	}
}

void SetStepperSteps(Stepper_t *Stepper, uint32_t TargetSteps)
{
	Stepper -> TargetSteps = TargetSteps;
}

void StepperForward(Stepper_t *Stepper, uint32_t Steps)
{
	Stepper -> TargetSteps += Steps;
}

void StepperBackward(Stepper_t *Stepper, uint32_t Steps)
{
	Stepper -> TargetSteps -= Steps;
}

#endif
