/*
 * GPIO.c
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */

#include "GPIO.h"

__attribute__((always_inline)) inline void PullUpGPIO(GPIO_t *GPIO)
{
	LL_GPIO_SetOutputPin(GPIO -> Port, GPIO -> Pin);
}

__attribute__((always_inline)) inline void PullDownGPIO(GPIO_t *GPIO)
{
	LL_GPIO_ResetOutputPin(GPIO -> Port, GPIO -> Pin);
}
