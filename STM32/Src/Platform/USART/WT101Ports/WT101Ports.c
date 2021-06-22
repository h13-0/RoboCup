/*
 * WT101Ports.c
 *
 *  Created on: 2021Äê6ÔÂ23ÈÕ
 *      Author: h13
 */
#include "WT101Ports.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

//Configs
#define target USART2

/**
 * @brief: Send data to WT101 through serialPort
 * @param: data
 */
__attribute__((always_inline)) inline void WT101SerialSend(uint8_t data)
{
	while (LL_USART_IsActiveFlag_TC(target) == RESET);
	LL_USART_TransmitData8(target, data);
}
