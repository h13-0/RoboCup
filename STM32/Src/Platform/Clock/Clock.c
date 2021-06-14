/*
 * Clock.c
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#include "Clock.h"

#include <stdint.h>

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_hal.h"
#endif

/**
 * @brief: Initialize the clock system.
 */
void ClockInit(void)
{
	//Nothing to do.
}

/**
 * @brief: The time delay function.
 * @param: delay time.
 */
inline void DelayMS(uint32_t delay)
{
	HAL_Delay(delay);
}

/**
 * @brief: Gets the system runtime.
 * @return: Runtime in ms.
 */
inline uint32_t GetMS(void)
{
	return HAL_GetTick();
}
