/*
 * VL6180X_GPIO_Ports.h
 *
 *  Created on: 2021Äê8ÔÂ11ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_GPIO_VL6180X_GPIO_PORTS_H_
#define PLATFORM_GPIO_VL6180X_GPIO_PORTS_H_

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_gpio.h"
#endif

typedef struct
{
	GPIO_TypeDef *XshutPort;             /*!< XShut GPIO Port */
	uint32_t      XshutPin;              /*!< Xshut GPIO Pin  */
} GPIO_t;

void VL6180x_GPIO_Reset(GPIO_t* GPIO);

void VL6180x_GPIO_Shutdown(GPIO_t* GPIO);

void VL6180x_GPIO_StartUp(GPIO_t* GPIO);

#endif /* PLATFORM_GPIO_VL6180X_GPIO_PORTS_H_ */
