/*
 * LuatPorts.h
 *
 *  Created on: 2021Äê8ÔÂ18ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_USART_LUATPORTS_LUATPORTS_H_
#define PLATFORM_USART_LUATPORTS_LUATPORTS_H_

#include <stdint.h>

/**
 * @brief: Send data to Luat 4G module through serial.
 * @param: data
 */
void LuatSerialSend(uint8_t data);

#endif /* PLATFORM_USART_LUATPORTS_LUATPORTS_H_ */
