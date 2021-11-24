/*
 * IdentifyFruit.c
 *
 *  Created on: 2021Äê11ÔÂ22ÈÕ
 *      Author: h13
 */
#include "IdentifyFruit.h"
#include "Drivers.h"

void IdentifyFruit(void)
{
	SwitchImageProcessingModuleWorkingMode(FruitDetection);
	SleepMillisecond(2500);



	while(1);
}
