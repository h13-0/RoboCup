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
				GPIO_PullUp(&(steppers[i] -> DirectionIO));
				GPIO_PullUp(&(steppers[i] -> StepperIO));
				steppers[i] -> CurrentSteps ++;
			} else if(steppers[i] -> CurrentSteps > steppers[i] -> TargetSteps) {

				GPIO_PullDown(&(steppers[i] -> DirectionIO));
				GPIO_PullUp(&(steppers[i] -> StepperIO));
				steppers[i] -> CurrentSteps --;
			}
		}
	} else {
		oddEvenSteps = 1;
		for(uint8_t i = 0; i < currentStepperNumbers; i++)
		{
			if(steppers[i] -> CurrentSteps < steppers[i] -> TargetSteps)
			{
				GPIO_PullUp(&(steppers[i] -> DirectionIO));
				GPIO_PullDown(&(steppers[i] -> StepperIO));
			} else if(steppers[i] -> CurrentSteps > steppers[i] -> TargetSteps) {
				GPIO_PullDown(&(steppers[i] -> DirectionIO));
				GPIO_PullDown(&(steppers[i] -> StepperIO));
			}
		}
	}
}

StepperError_t StepperInit(Stepper_t *Stepper)
{
	GPIO_Init(&Stepper -> StepperIO, Output);
	GPIO_Init(&Stepper -> DirectionIO, Output);
	if(currentStepperNumbers < StepperNumbers)
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

void StepperForward(Stepper_t *Stepper, int32_t Steps)
{
	Stepper -> TargetSteps += Steps;
}

void StepperBackward(Stepper_t *Stepper, int32_t Steps)
{
	Stepper -> TargetSteps -= Steps;
}

#endif
