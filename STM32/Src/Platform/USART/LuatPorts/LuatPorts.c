/*
 * LuatPorts.c
 *
 *  Created on: 2021Äê8ÔÂ18ÈÕ
 *      Author: h13
 */
#include "LuatPorts.h"
#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

/**
 * @brief: Send data to Luat 4G module through serial.
 * @param: data
 */
__attribute__((always_inline)) inline void LuatSerialSend(uint8_t data)
{
	while (LL_USART_IsActiveFlag_TC(LuatPorts) == RESET);
	LL_USART_TransmitData8(LuatPorts, data);
}
