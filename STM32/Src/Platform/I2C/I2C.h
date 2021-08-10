/*
 * I2C.h
 *
 *  Created on: 2021Äê6ÔÂ14ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_I2C_I2C_H_
#define PLATFORM_I2C_I2C_H_

#define HAL               (0)
#define LL                (1)
#define STM_I2C_LIB_TYPE  HAL

typedef enum
{
	I2C_OK = 0,
	I2C_Busy,
	I2C_HardwareFault,
	I2C_SlaveNotFound,
	I2C_TransmissionTimeout,
} I2C_Error_t;

#if(STM_I2C_LIB_TYPE == HAL)

#if defined(STM32F103xB) || defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_hal.h"
#endif

void I2C_SetPort(I2C_HandleTypeDef *Port);

#elif(STM_I2C_LIB_TYPE == LL)
#if defined(STM32F103xB) || defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_i2c.h"
#endif

void I2C_SetPort(I2C_TypeDef *port);

#endif


I2C_Error_t I2C_WriteByte(uint8_t Address, uint8_t Register, uint8_t Byte);

I2C_Error_t I2C_ReadByte(uint8_t Address, uint8_t Register, uint8_t *Byte);

I2C_Error_t I2C_WriteLen(uint8_t Address, uint8_t Register, uint8_t Length, uint8_t *Buffer);

I2C_Error_t I2C_ReadLen(uint8_t Address, uint8_t Register, uint8_t Length, uint8_t *Buffer);

I2C_Error_t I2C_Transmit(uint8_t Address, uint8_t *Buffer, uint8_t Length);

I2C_Error_t I2C_Receive(uint8_t Address, uint8_t *Buffer, uint8_t Length);

#endif /* PLATFORM_I2C_I2C_H_ */
