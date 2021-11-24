/*
 * USART.h
 *
 *  Created on: 2021Äê9ÔÂ3ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_USART_USART_H_
#define PLATFORM_USART_USART_H_

#include "RobotConfigs.h"
#include <stdint.h>

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

void SerialInit(USART_t USART, uint32_t BaudRate);

void SerialSend(USART_t USART, uint8_t data);

#endif /* PLATFORM_USART_USART_H_ */
