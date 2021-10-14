/*
 * GPIO.c
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */

#include "GPIO.h"

__attribute__((always_inline)) inline void GPIO_Init(GPIO_t *GPIO, GPIO_Function_t Function) { }

__attribute__((always_inline)) inline void GPIO_PullUp(GPIO_t *GPIO)
{
	LL_GPIO_SetOutputPin(GPIO -> Port, GPIO -> Pin);
}

__attribute__((always_inline)) inline void GPIO_PullDown(GPIO_t *GPIO)
{
	LL_GPIO_ResetOutputPin(GPIO -> Port, GPIO -> Pin);
}

__attribute__((always_inline)) inline uint32_t GPIO_ReadLevel(GPIO_t *GPIO)
{
	return LL_GPIO_IsInputPinSet(GPIO -> Port, GPIO -> Pin);
}
