/*
 * I2C.c
 *
 *  Created on: 2021Äê6ÔÂ14ÈÕ
 *      Author: h13
 */

#include "I2C.h"
#include <stddef.h>

#if(STM_I2C_LIB_TYPE == HAL)

#define I2C_RETRY   3
#define I2C_TIMEOUT 300

static I2C_HandleTypeDef *i2cPort = NULL;

void I2C_SetPort(I2C_HandleTypeDef *Port)
{
	i2cPort = Port;
}

I2C_Error_t I2C_WriteByte(uint8_t Address, uint8_t Register, uint8_t Byte)
{
	if(HAL_I2C_IsDeviceReady(i2cPort, Address*2, I2C_RETRY, I2C_TIMEOUT) == HAL_OK)
	{
		if(HAL_I2C_Mem_Write(i2cPort, Address*2, Register, I2C_MEMADD_SIZE_8BIT, &Byte, 1, I2C_TIMEOUT) == HAL_OK)
		{
			return I2C_OK;
		} else {
			return I2C_TransmissionTimeout;
		}
	} else {
		return I2C_SlaveNotFound;
	}
}

I2C_Error_t I2C_ReadByte(uint8_t Address, uint8_t Register, uint8_t *Byte)
{
	if(HAL_I2C_IsDeviceReady(i2cPort, (Address * 2) + 1, I2C_RETRY, I2C_TIMEOUT) == HAL_OK)
	{
		if(HAL_I2C_Mem_Read(i2cPort, (Address * 2) + 1, Register, I2C_MEMADD_SIZE_8BIT, Byte, 1, I2C_TIMEOUT) == HAL_OK)
		{
			return I2C_OK;
		} else {
			return I2C_TransmissionTimeout;
		}
	} else {
		return I2C_SlaveNotFound;
	}
}

I2C_Error_t I2C_WriteLen(uint8_t Address, uint8_t Register, uint8_t Length, uint8_t *Buffer)
{
	if(HAL_I2C_IsDeviceReady(i2cPort, Address * 2, I2C_RETRY, I2C_TIMEOUT) == HAL_OK)
	{
		if(HAL_I2C_Mem_Write(i2cPort, Address * 2, Register, I2C_MEMADD_SIZE_8BIT, Buffer, Length, I2C_TIMEOUT) == HAL_OK)
		{
			return I2C_OK;
		} else {
			return I2C_TransmissionTimeout;
		}
	} else {
		return I2C_SlaveNotFound;
	}
}

I2C_Error_t I2C_ReadLen(uint8_t Address, uint8_t Register, uint8_t Length, uint8_t *Buffer)
{
	if(HAL_I2C_IsDeviceReady(i2cPort, (Address * 2) + 1, I2C_RETRY, I2C_TIMEOUT) == HAL_OK)
	{
		if(HAL_I2C_Mem_Read(i2cPort, (Address * 2) + 1, Register, I2C_MEMADD_SIZE_8BIT, Buffer, Length, I2C_TIMEOUT) == HAL_OK)
		{
			return I2C_OK;
		} else {
			return I2C_TransmissionTimeout;
		}
	} else {
		return I2C_SlaveNotFound;
	}
}

/**
 *
 * HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
 */


I2C_Error_t I2C_Transmit(uint8_t Address, uint8_t *Buffer, uint8_t Length)
{
	HAL_StatusTypeDef ret = HAL_OK;
	ret = HAL_I2C_Master_Transmit(i2cPort, Address, Buffer, Length , I2C_TIMEOUT);
	switch(ret)
	{
	case HAL_OK:
		return I2C_OK;

	case HAL_ERROR:
		return I2C_HardwareFault;

	case HAL_BUSY:
		return I2C_Busy;

	case HAL_TIMEOUT:
		return I2C_TransmissionTimeout;
	}
	return I2C_OK;
}

I2C_Error_t I2C_Receive(uint8_t Address, uint8_t *Buffer, uint8_t Length)
{
	HAL_StatusTypeDef ret = HAL_OK;
	ret = HAL_I2C_Master_Receive(i2cPort, Address, Buffer, Length , I2C_TIMEOUT);
	switch(ret)
	{
	case HAL_OK:
		return I2C_OK;

	case HAL_ERROR:
		return I2C_HardwareFault;

	case HAL_BUSY:
		return I2C_Busy;

	case HAL_TIMEOUT:
		return I2C_TransmissionTimeout;
	}
	return I2C_OK;
}

#elif(STM_I2C_LIB_TYPE == LL)

static I2C_TypeDef *i2cport = NULL;

#define errorTimeLimit 1000
#define wait(logicalExpression, returnCode) { uint16_t errorTimes = 0; while(logicalExpression){ errorTimes ++; if(errorTimes > errorTimeLimit){ return returnCode; }}}

void I2C_SetPort(I2C_TypeDef *port)
{
	i2cport = port;
}

I2C_Error_t I2C_WriteByte(uint8_t address, uint8_t reg, uint8_t data)
{
	//Wait
	wait(LL_I2C_IsActiveFlag_BUSY(i2cport) == SET, I2C_Busy);

	//Start
	LL_I2C_GenerateStartCondition(i2cport);
	wait(LL_I2C_IsActiveFlag_SB(i2cport) == RESET, I2C_HardwareFault);

	//Address
	LL_I2C_TransmitData8(i2cport, address * 2);
	wait (LL_I2C_IsActiveFlag_ADDR(i2cport) == RESET, I2C_SlaveNotFound);
	LL_I2C_ClearFlag_ADDR(i2cport);

	//Reg
	LL_I2C_TransmitData8(i2cport, reg);
	wait(LL_I2C_IsActiveFlag_TXE(i2cport) == RESET, I2C_TransmissionTimeout);

	//Transmission
	LL_I2C_TransmitData8(i2cport, data);
	wait(LL_I2C_IsActiveFlag_TXE(i2cport) == RESET, I2C_TransmissionTimeout);

	LL_I2C_GenerateStopCondition(i2cport);

	return I2C_OK;
}

I2C_Error_t I2C_ReadByte(uint8_t address, uint8_t reg, uint8_t *data)
{
	//Wait
	wait(LL_I2C_IsActiveFlag_BUSY(i2cport) == SET, I2C_Busy);

	//Start
	LL_I2C_GenerateStartCondition(i2cport);
	wait(LL_I2C_IsActiveFlag_SB(i2cport) == RESET, I2C_HardwareFault);

	//Address
	LL_I2C_TransmitData8(i2cport, address * 2);
	wait(LL_I2C_IsActiveFlag_ADDR(i2cport) == RESET, I2C_SlaveNotFound);
	LL_I2C_ClearFlag_ADDR(i2cport);

	//Reg
	LL_I2C_TransmitData8(i2cport, reg);
	wait(LL_I2C_IsActiveFlag_TXE(i2cport) == RESET, I2C_TransmissionTimeout);

	//Restart
	LL_I2C_GenerateStartCondition(i2cport);
	wait(LL_I2C_IsActiveFlag_SB(i2cport) == RESET, I2C_HardwareFault);

	//Address
	LL_I2C_TransmitData8(i2cport, address * 2 + 1);
	wait(LL_I2C_IsActiveFlag_ADDR(i2cport) == RESET, I2C_SlaveNotFound);
	LL_I2C_ClearFlag_ADDR(i2cport);

	//Read
	//Stop after read.
	LL_I2C_GenerateStopCondition(i2cport);
	LL_I2C_AcknowledgeNextData(i2cport, LL_I2C_NACK);
	wait(LL_I2C_IsActiveFlag_RXNE(i2cport) == RESET, I2C_TransmissionTimeout);
	*data = LL_I2C_ReceiveData8(i2cport);

	return I2C_OK;
}


I2C_Error_t I2C_WriteLen(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf) {
	//Wait
	wait(LL_I2C_IsActiveFlag_BUSY(i2cport) == SET, I2C_Busy);

	//Start
	LL_I2C_GenerateStartCondition(i2cport);
	wait(LL_I2C_IsActiveFlag_SB(i2cport) == RESET, I2C_HardwareFault);

	//Address
	LL_I2C_TransmitData8(i2cport, address * 2);
	wait(LL_I2C_IsActiveFlag_ADDR(i2cport) == RESET, I2C_SlaveNotFound);

	LL_I2C_ClearFlag_ADDR(i2cport);

	//Reg
	LL_I2C_TransmitData8(i2cport, reg);
	wait(LL_I2C_IsActiveFlag_TXE(i2cport) == RESET, I2C_TransmissionTimeout);

	//data
	for (uint8_t i = 0; i < len; i++) {
		LL_I2C_TransmitData8(i2cport, buf[i]);
		wait(LL_I2C_IsActiveFlag_TXE(i2cport) == RESET, I2C_TransmissionTimeout);
	}

	LL_I2C_GenerateStopCondition(i2cport);
	return I2C_OK;
}

I2C_Error_t I2C_ReadLen(uint8_t address, uint8_t reg, uint8_t len, uint8_t *buf) {
	//Wait
	wait(LL_I2C_IsActiveFlag_BUSY(i2cport) == SET, I2C_Busy);

	//Start
	LL_I2C_GenerateStartCondition(i2cport);
	wait(LL_I2C_IsActiveFlag_SB(i2cport) == RESET, I2C_HardwareFault);

	//Address
	LL_I2C_TransmitData8(i2cport, address * 2);
	wait(LL_I2C_IsActiveFlag_ADDR(i2cport) == RESET, I2C_SlaveNotFound);

	LL_I2C_ClearFlag_ADDR(i2cport);

	//Reg
	LL_I2C_TransmitData8(i2cport, reg);
	wait(LL_I2C_IsActiveFlag_TXE(i2cport) == RESET, I2C_TransmissionTimeout);

	//Restart
	//LL_I2C_GenerateStopCondition(i2cport);
	//wait(LL_I2C_IsActiveFlag_SB(i2cport) == SET, I2C_HardwareFault);
	LL_I2C_GenerateStartCondition(i2cport);
	wait(LL_I2C_IsActiveFlag_SB(i2cport) == RESET, I2C_HardwareFault);

	//Address
	LL_I2C_TransmitData8(i2cport, address * 2 + 1);
	wait(LL_I2C_IsActiveFlag_ADDR(i2cport) == RESET, I2C_SlaveNotFound);

	LL_I2C_ClearFlag_ADDR(i2cport);

	//Read
	for (uint8_t i = len; i > 0; i--) {
		if (i == 1) {
			LL_I2C_GenerateStopCondition(i2cport);
			LL_I2C_AcknowledgeNextData(i2cport, LL_I2C_NACK);
		} else {
			LL_I2C_AcknowledgeNextData(i2cport, LL_I2C_ACK);
		}

		wait(LL_I2C_IsActiveFlag_RXNE(i2cport) == RESET, I2C_TransmissionTimeout);

		*buf = LL_I2C_ReceiveData8(i2cport);
		buf++;
	}
	return I2C_OK;
}
#endif
