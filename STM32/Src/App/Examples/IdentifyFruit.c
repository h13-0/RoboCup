/*
 * IdentifyFruit.c
 *
 *  Created on: 2021Äê11ÔÂ22ÈÕ
 *      Author: h13
 */
#include "IdentifyFruit.h"
#include "Drivers.h"
#include "Examples.h"

#include "UI.h"

void IdentifyFruit(void)
{
	SwitchImageProcessingModuleWorkingMode(FruitIdentify);
	SleepMillisecond(2500);

	AimAt(FruitFocus, 10000);

	while(1)
	{
		FruitIdentifyResult_t result = GetFruitDetectionResult();

		SendResultViaMessage(&result);
		DisplayResult(&result);
	}
}
