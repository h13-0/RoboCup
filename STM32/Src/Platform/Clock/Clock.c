/*
 * Clock.c
 *
 *  Created on: 2021年6月25日
 *      Author: h13
 */
#include "Clock.h"

#if defined(STM32F103xB) || defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx.h"
#endif

static mtime_t _milliseconds = 0;
static uint32_t microsecondProportion = 0;

/**
 * @brief: Systick interrupt function handle.
 * @note:  Place it in the systick function with a period of 1ms.
 */
__attribute__((always_inline)) inline void SysTickInterruptHandler(void)
{
	_milliseconds ++;
}

void ClockInit(void)
{
	microsecondProportion = (SysTick -> LOAD + 1) / 1000;
	SysTick -> CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

/**
 * @brief: Sleep milliseconds.
 * @param: Milliseconds.
 */
void SleepMillisecond(mtime_t milliseconds)
{
	uint32_t startVal = SysTick -> VAL;
	mtime_t startMillisecond = _milliseconds;

	while(startMillisecond + milliseconds > _milliseconds);
	while(startVal < SysTick -> VAL);
}

/**
 * @brief: Sleep milliseconds.
 * @param: Microseconds.
 * 		Microseconds = 1 -> 2.5
 * 		3.5
 * 		4 -> 5--5.5
 * 		5 -> 6.5
 */
void SleepMicrosecond(mtime_t microseconds)
{
#if defined(STM32F103xB) || defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
	if(microseconds > 1)
	{
		microseconds --;
	}
#endif

	uint32_t startVal = SysTick -> VAL;
	mtime_t endMillsecond = _milliseconds;
	uint32_t endVal = 0;

	uint64_t ticks = microseconds * microsecondProportion - 1;
	if(startVal < ticks)
	{
		endMillsecond = endMillsecond + 1 + ticks / (SysTick -> LOAD + 1);
		endVal = (SysTick -> LOAD - (ticks % (SysTick -> LOAD + 1)));
	} else {
		endVal = startVal - microseconds * microsecondProportion;
	}

	while(endMillsecond > _milliseconds);

	while((SysTick -> VAL > endVal) && (endMillsecond == _milliseconds));
}

/**
 * @brief:  Get system running time.
 * @return: Time in Milliseconds.
 */
__attribute__((always_inline)) inline mtime_t GetCurrentTimeMillisecond(void)
{
	return _milliseconds;
}

/**
 * @brief:  Get system running time.
 * @return: Time in Microseconds.
 */
__attribute__((always_inline)) inline mtime_t GetCurrentTimeMicrosecond(void)
{
	return _milliseconds * 1000 + (SysTick -> LOAD + 1 - SysTick -> VAL) / microsecondProportion;
}
