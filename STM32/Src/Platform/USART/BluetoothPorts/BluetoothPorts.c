/*
 * BluetoothPorts.c
 *
 *  Created on: 2021Äê8ÔÂ23ÈÕ
 *      Author: h13
 */
#include "BluetoothPorts.h"
#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

/**
 * @brief: Send data to bluetooth through serial port.
 * @param: data
 */
__attribute__((always_inline)) inline void BluetoothSerialSend(uint8_t data)
{
	while (LL_USART_IsActiveFlag_TC(BluetoothPorts) == RESET);
	LL_USART_TransmitData8(BluetoothPorts, data);
}
