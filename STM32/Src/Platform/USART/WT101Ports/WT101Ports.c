/*
 * WT101Ports.c
 *
 *  Created on: 2021Äê6ÔÂ23ÈÕ
 *      Author: h13
 */
#include "WT101Ports.h"
#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

/**
 * @brief: Send data to WT101 through serialPort
 * @param: data
 */
__attribute__((always_inline)) inline void WT101_SerialSend(uint8_t data)
{
	while (LL_USART_IsActiveFlag_TC(WT101_Ports) == RESET);
	LL_USART_TransmitData8(WT101_Ports, data);
}
