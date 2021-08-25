/*
 * GPIO.h
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_GPIO_GPIO_H_
#define PLATFORM_GPIO_GPIO_H_

#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_gpio.h"
#endif

typedef struct
{
	GPIO_Ports_t *Port;
	GPIO_Pin_t    Pin;
} GPIO_t;

void PullUpGPIO(GPIO_t *GPIO);

void PullDownGPIO(GPIO_t *GPIO);

//ReadGPIO_State();

#endif /* PLATFORM_GPIO_GPIO_H_ */
