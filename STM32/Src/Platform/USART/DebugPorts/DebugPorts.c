/*
 * DebugPorts.c
 *
 *  Created on: 2021Äê6ÔÂ23ÈÕ
 *      Author: h13
 */

#include "DebugPorts.h"
#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

/**
 * @brief: Send debug data through serialPort
 * @param: data
 */
__attribute__((always_inline)) inline void LogSerialSend(uint8_t data)
{
	while (LL_USART_IsActiveFlag_TC(DebugPorts) == RESET);
	LL_USART_TransmitData8(DebugPorts, data);
}

#ifdef __GNUC__
int __io_putchar(int ch) {
	LogSerialSend(ch);
	return ch;
}
#else
int fputc(int ch, FILE *stream)
{
	LogSerialSend(ch);
	return ch;
}
#endif



