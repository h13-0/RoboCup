/*
 * INA219.c
 *
 *  Created on: Dec 30, 2020
 *       Author: Piotr Smolen <komuch@gmail.com>
 *
 * @note: This library has been simplified.
 * 		  The original library address is https://github.com/komuch/PSM_INA219_STM32/tree/f7414a94c20301b339bad2710239226bcdeb87e4
 */

#include "INA219.h"
#include "I2C.h"
#include "ports.h"

static uint16_t read16(uint8_t Address, uint8_t Register)
{
	uint8_t value[2];

	I2C_ReadLen(Address, Register, 2, value);

	return ((value[0] << 8) | value[1]);
}

static void write16(uint8_t Address, uint8_t Register, uint16_t Value)
{
	uint8_t addr[2];
	addr[0] = (Value >> 8) & 0xff;  // upper byte
	addr[1] = (Value >> 0) & 0xff; // lower byte
	I2C_WriteLen(Address, Register, 2, addr);
}

uint16_t INA219_ReadBusVoltage(uint8_t Address)
{
	uint16_t result = read16(Address, INA219_REG_BUSVOLTAGE);

	return ((result >> 3  ) * 4);
}

int16_t INA219_ReadCurrent_raw(uint8_t Address)
{
	int16_t result = read16(Address, INA219_REG_CURRENT);

	return (result);
}

int16_t INA219_ReadCurrent(uint8_t Address)
{
	int16_t result = INA219_ReadCurrent_raw(Address);

	return (result / ina219_currentDivider_mA);
}

uint16_t INA219_ReadShuntVolage(uint8_t Address)
{
	uint16_t result = read16(Address, INA219_REG_SHUNTVOLTAGE);

	return (result * 0.01 );
}

void INA219_Reset(uint8_t Address)
{
	write16(Address, INA219_REG_CONFIG, INA219_CONFIG_RESET);
	SleepMillisecond(1);
}

void INA219_SetCalibration(uint8_t Address, uint16_t CalibrationData)
{
	write16(Address, INA219_REG_CALIBRATION, CalibrationData);
}

uint16_t INA219_GetConfig(uint8_t Address)
{
	uint16_t result = read16(Address, INA219_REG_CONFIG);
	return result;
}

void INA219_SetConfig(uint8_t Address, uint16_t Config)
{
	write16(Address, INA219_REG_CONFIG, Config);
}

void INA219_SetCalibration_32V_2A(uint8_t Address)
{
	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
	             INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
	             INA219_CONFIG_SADCRES_12BIT_1S_532US |
	             INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

	ina219_calibrationValue = 4096;
	ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
	ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)

	INA219_SetCalibration(Address, ina219_calibrationValue);
	INA219_SetConfig(Address, config);
}

void INA219_SetCalibration_32V_1A(uint8_t Address)
{
	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
	                    INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
	                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
	                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

	ina219_calibrationValue = 10240;
	ina219_currentDivider_mA = 25;    // Current LSB = 40uA per bit (1000/40 = 25)
	ina219_powerMultiplier_mW = 0.8f; // Power LSB = 800uW per bit

	INA219_SetCalibration(Address, ina219_calibrationValue);
	INA219_SetConfig(Address, config);
}

void INA219_SetCalibration_16V_400mA(uint8_t Address)
{
	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
	                    INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT |
	                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
	                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

	ina219_calibrationValue = 8192;
	ina219_currentDivider_mA = 20;    // Current LSB = 50uA per bit (1000/50 = 20)
	ina219_powerMultiplier_mW = 1.0f; // Power LSB = 1mW per bit

	INA219_SetCalibration(Address, ina219_calibrationValue);
	INA219_SetConfig(Address, config);
}

void INA219_SetPowerMode(uint8_t Address, uint8_t Mode)
{
	uint16_t config = INA219_GetConfig(Address);

	switch (Mode) {
		case INA219_CONFIG_MODE_POWERDOWN:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_POWERDOWN & INA219_CONFIG_MODE_MASK);
			INA219_SetConfig(Address, config);
			break;

		case INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED & INA219_CONFIG_MODE_MASK);
			INA219_SetConfig(Address, config);
			break;

		case INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS & INA219_CONFIG_MODE_MASK);
			INA219_SetConfig(Address, config);
			break;

		case INA219_CONFIG_MODE_ADCOFF:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_ADCOFF & INA219_CONFIG_MODE_MASK);
			INA219_SetConfig(Address, config);
			break;
	}
}

void INA219_Init(uint8_t Address)
{
	ina219_currentDivider_mA = 0;
	ina219_powerMultiplier_mW = 0;

	INA219_Reset(Address);
	INA219_SetCalibration_32V_2A(Address);
}
