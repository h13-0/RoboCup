/*
 * HC05_GPIO_Ports.c
 *
 *  Created on: 2021Äê8ÔÂ23ÈÕ
 *      Author: h13
 */
#include "HC05_GPIO_Ports.h"
#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_gpio.h"
#endif

void SetHC05_EN_Pin(void)
{
	LL_GPIO_SetOutputPin(BT05_Port, BT05_Pin);
}

void ResetHC05_EN_Pin(void)
{
	LL_GPIO_ResetOutputPin(BT05_Port, BT05_Pin);
}
