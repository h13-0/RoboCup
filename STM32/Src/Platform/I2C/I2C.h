/*
 * I2C.h
 *
 *  Created on: 2021Äê6ÔÂ14ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_I2C_I2C_H_
#define PLATFORM_I2C_I2C_H_

#if defined(STM32F103xB) || defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_i2c.h"
#endif

typedef enum
{
	I2C_OK = 0,
	I2C_Busy,
	I2C_HardwareFault,
	I2C_SlaveNotFound,
	I2C_TransmissionTimeout,
} I2C_Error_t;

void I2C_SetPort(I2C_TypeDef *port);

I2C_Error_t I2C_WriteByte(uint8_t address, uint8_t reg, uint8_t data);

I2C_Error_t I2C_ReadByte(uint8_t address, uint8_t reg, uint8_t *data);

I2C_Error_t I2C_WriteLen(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf);

I2C_Error_t I2C_ReadLen(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf);


#endif /* PLATFORM_I2C_I2C_H_ */
