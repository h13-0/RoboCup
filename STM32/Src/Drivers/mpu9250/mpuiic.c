#include "mpuiic.h"

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_hal.h"
#endif

static I2C_HandleTypeDef *i2cport = NULL;

/**
 * @brief: 设定I2C端口
 * @return: void
 */
void MPU_Set_IIC_Port(I2C_HandleTypeDef *port) {
	i2cport = port;
}

/**
 * @brief: I2C写一个字节
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * 		uint8_t data: 待写入数据
 * @return: 错误码, 成功为0
 */
uint8_t MPU_Write_Byte(uint8_t address, uint8_t reg, uint8_t data) {
	if (HAL_I2C_IsDeviceReady(i2cport, address*2, I2C_RETRY, I2C_TIMEOUT)
			== HAL_OK) {
		if (HAL_I2C_Mem_Write(i2cport, address*2, reg, I2C_MEMADD_SIZE_8BIT,
				&data, 1, I2C_TIMEOUT) == HAL_OK) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 2;
	}

}

/**
 * @brief: I2C读一个字节
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * @return: 获取到的数据
 */

uint8_t MPU_Read_Byte(uint8_t address, uint8_t reg) {
	uint8_t data = 0;
	HAL_I2C_IsDeviceReady(i2cport, (address * 2) + 1, I2C_RETRY, I2C_TIMEOUT);
	HAL_I2C_Mem_Read(i2cport, (address * 2) + 1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1,
	I2C_TIMEOUT);
	return data;
}

/**
 * @brief: I2C连续写
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * 		uint8_t len: 要写入的长度
 * 		uint8_t *buf: 要写入的数据
 * @return: 错误码, 成功为0
 */
uint8_t MPU_Write_Len(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf) {
	if (HAL_I2C_IsDeviceReady(i2cport, address * 2, I2C_RETRY, I2C_TIMEOUT)
			== HAL_OK) {
		if (HAL_I2C_Mem_Write(i2cport, address * 2, reg,
		I2C_MEMADD_SIZE_8BIT, buf, len, I2C_TIMEOUT) == HAL_OK) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 2;
	}
}

/**
 * @brief: I2C连续读
 * @param:
 * 		uint8_t address: 目标I2C地址
 * 		uint8_t reg: 寄存器地址
 * 		uint8_t len: 要读取的长度
 * 		uint8_t *buf: 要读取的数据缓冲区
 * @return: 错误码, 成功为0
 */
uint8_t MPU_Read_Len(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf) {
	if (HAL_I2C_IsDeviceReady(i2cport, (address * 2) + 1, I2C_RETRY, I2C_TIMEOUT)
			== HAL_OK) {
		if (HAL_I2C_Mem_Read(i2cport, (address * 2) + 1, reg,
		I2C_MEMADD_SIZE_8BIT, buf, len, I2C_TIMEOUT) == HAL_OK) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 2;
	}
}
