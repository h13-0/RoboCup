#ifndef __MPUIIC_H
#define __MPUIIC_H

#define I2C_RETRY   3
#define I2C_TIMEOUT 1000

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_hal.h"
#endif

/**
 * @brief: 设定I2C端口
 * @return: void
 */
void MPU_Set_IIC_Port(I2C_HandleTypeDef *port);


/**
 * @brief: I2C写一个字节
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * 		uint8_t data: 待写入数据
 * @return: 错误码, 成功为0
 */
uint8_t MPU_Write_Byte(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief: I2C读一个字节
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * @return: 获取到的数据
 */

uint8_t MPU_Read_Byte(uint8_t address, uint8_t reg);

/**
 * @brief: I2C连续写
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * 		uint8_t len: 要写入的长度
 * 		uint8_t *buf: 要写入的数据
 * @return: 错误码, 成功为0
 */
uint8_t MPU_Write_Len(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf);

/**
 * @brief: I2C连续读
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * 		uint8_t len: 要读取的长度
 * 		uint8_t *buf: 要读取的数据缓冲区
 * @return: 错误码, 成功为0
 */
uint8_t MPU_Read_Len(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf);

#endif
