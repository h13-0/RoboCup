/*
 * IdentifyFruit.c
 *
 *  Created on: 2021Äê11ÔÂ22ÈÕ
 *      Author: h13
 */
#include "IdentifyFruit.h"
#include <string.h>
#include "Drivers.h"
#include "Examples.h"

#include "UI.h"

typedef struct
{
	FruitIdentifyResult_t Result;
	int8_t Count;
} FruitIdentifyResultCount_t;

static FruitIdentifyResultCount_t resultList[3] = { 0 };

static uint8_t isSameResult(FruitIdentifyResult_t *Result1, FruitIdentifyResult_t *Result2)
{
	return !memcmp(Result1, Result2, sizeof(FruitIdentifyResult_t));
}

void IdentifyFruit(void)
{
	SwitchImageProcessingModuleWorkingMode(FruitIdentify);

	AimAt(FruitFocus, 10000);

	mtime_t startTime = GetCurrentTimeMillisecond();
	FruitIdentifyResult_t result;
	while(GetCurrentTimeMillisecond() - startTime < 3000)
	{
		result = GetFruitDetectionResult();

		for(int8_t index = 0; index < 3; index ++)
		{
			if(resultList[index].Count < 0)
			{
				resultList[index].Result = result;
				resultList[index].Count = 1;
				break;
			} else if(isSameResult(&result, &resultList[index].Result))
			{
				resultList[index].Count += 2;
				break;
			}
		}

		for(int8_t index = 0; index < 3; index ++)
		{
			resultList[index].Count --;
		}
		SleepMillisecond(100);
	}

	//Find highest frequency result.
	int8_t maxID = 0;
	int8_t maxCount = - 128;

	for(int8_t index = 0; index < 3; index ++)
	{
		if(resultList[index].Count >= maxCount)
		{
			maxCount = resultList[index].Count;
			maxID = index;
		}
	}
	result = resultList[maxID].Result;

	SendResultViaMessage(&result);
	DisplayResult(&result);
	BroadcastIdentifyResult(&result);

	SmoothMoveTo(MoveZ_AxisHeight, ApproachHeight, 5);
	SmoothMoveTo(MoveAxialLength, AL_AxisZeroPoint, 5);
}
