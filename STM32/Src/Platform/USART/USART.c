/*
 * USART.c
 *
 *  Created on: 2021Äê9ÔÂ3ÈÕ
 *      Author: h13
 */

#include "USART.h"
#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

void SerialInit(USART_t USART, uint32_t BaudRate)
{
	//Do nothing in low-layer library.
}

__attribute__((always_inline)) inline void SerialSend(USART_t USART, uint8_t data)
{
	while (LL_USART_IsActiveFlag_TC(USART) == RESET);
	LL_USART_TransmitData8(USART, data);
}
