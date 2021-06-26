/*
 * Clock.h
 *
 *  Created on: 2021年6月25日
 *      Author: h13
 */

#ifndef SRC_CLOCK_CLOCK_H_
#define SRC_CLOCK_CLOCK_H_

#include <stdint.h>

/**
 * @group: Select time typedef
 * @option:
 * 		@TimeTypedef32: The longest stable working time is 1.19 hours.
 * 		@TimeTypedef64: The longest stable working time is 583000 years.
 */
#define TimeTypedef32 0
#define TimeTypedef64 1
#define TimeType TimeTypedef32

#if(TimeType == TimeTypedef32)
typedef uint32_t mtime_t;
#elif(TimeType == TimeTypedef64)
typedef mtime_t uint64_t;
#endif

void ClockInit(void);

/**
 * @brief: Systick interrupt function handle.
 * @note:  Place it in the systick function with a period of 1ms.
 */
void SysTickInterruptHandler(void);

/**
 * @brief: Sleep milliseconds.
 * @param: Milliseconds.
 */
void SleepMillisecond(mtime_t milliseconds);

/**
 * @brief: Sleep milliseconds.
 * @param: Microseconds.
 */
void SleepMicrosecond(mtime_t microseconds);

/**
 * @brief:  Get system running time.
 * @return: Time in Milliseconds.
 */
mtime_t GetCurrentTimeMillisecond(void);

/**
 * @brief:  Get system running time.
 * @return: Time in Microseconds.
 */
mtime_t GetCurrentTimeMicrosecond(void);

#endif /* SRC_CLOCK_CLOCK_H_ */
