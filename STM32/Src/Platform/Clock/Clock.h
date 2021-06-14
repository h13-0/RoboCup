/*
 * Clock.h
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_CLOCK_CLOCK_H_
#define PLATFORM_CLOCK_CLOCK_H_

#include <stdint.h>

/**
 * @brief: Initialize the clock system.
 */
void ClockInit(void);

/**
 * @brief: The time delay function.
 * @param: Delay time.
 */
void DelayMS(uint32_t delay);

/**
 * @brief: Gets the system runtime.
 * @return: Runtime in ms.
 */
uint32_t GetMS(void);

#endif /* PLATFORM_CLOCK_CLOCK_H_ */
