/*
 * Bluetooth.c
 *
 *  Created on: 2021Äê8ÔÂ23ÈÕ
 *      Author: h13
 */
#include "Bluetooth.h"
#include "RobotConfigs.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "Ports.h"

static GPIO_t bleEN_IO = BluetoothEN_IO;

void BluetoothPrintf(char *fmt, ...)
{
	uint8_t length;
	char buffer[32];
	va_list arg;

	GPIO_PullDown(&bleEN_IO);

	va_start(arg, fmt);
	length = vsprintf(buffer, fmt, arg);
	va_end(arg);

	for(uint8_t i = 0; i < length; i++)
	{
		BluetoothSerialSend(buffer[i]);
	}
}

void BluetoothSendChar(char Data)
{
	BluetoothSerialSend(Data);
}

static void sendAT_Command(char *fmt, ...)
{
	uint8_t length;
	char buffer[32];
	va_list arg;

	GPIO_PullUp(&bleEN_IO);
	SleepMillisecond(10);

	va_start(arg, fmt);
	length = vsprintf(buffer, fmt, arg);
	va_end(arg);

	for(uint8_t i = 0; i < length; i++)
	{
		BluetoothSerialSend(buffer[i]);
	}

	SleepMillisecond(300);
	GPIO_PullDown(&bleEN_IO);
}

void ResetBluetoothConfigs(void)
{
	sendAT_Command("AT+ORGL\r\n");
}

void SetBluetoothBaundRate(HC05_BaundRate_t BaundRate, HC05_StopBit_t StopBit, HC05_CheckBit_t CheckBit)
{
	uint32_t baundRate = 38400;
	switch(BaundRate)
	{
	case HC05_2400bps:
		baundRate = 2400;
		break;
	case HC05_4800bps:
		baundRate = 4800;
		break;
	case HC05_9600bps:
		baundRate = 9600;
		break;
	case HC05_19200bps:
		baundRate = 19200;
		break;
	case HC05_38400bps:
		baundRate = 38400;
		break;
	case HC05_57600bps:
		baundRate = 57600;
		break;
	case HC05_115200bps:
		baundRate = 115200;
		break;
	case HC05_230400bps:
		baundRate = 230400;
		break;
	case HC05_460800bps:
		baundRate = 460800;
		break;
	case HC05_921600bps:
		baundRate = 921600;
		break;
	case HC05_1382400bps:
		baundRate = 1382400;
		break;
	default:
		break;
	}

	sendAT_Command("AT+UART=%d,%d,%d\r\n", baundRate, StopBit, CheckBit);
}

/**
 * @brief: Set pairing code.
 * @param: Code in 1 to 8 decimal digits.
 */
void SetBluetoothPin(char *Pin)
{
	if(strlen(Pin) <= 8)
	{
		sendAT_Command("AT+PSWD=\"%s\"\r\n", Pin);
	}
}

void SetBluetoothRole(ROLE_t Role)
{
	sendAT_Command("AT+ROLE=%d\r\n", Role);
}

void SetBluetoothClass(uint32_t Class)
{
	sendAT_Command("AT+CLASS=%d\r\n", Class);
}

void SetBluetoothConnectMode(CMODE_t ConnectMode)
{
	sendAT_Command("AT+CMODE=%d\r\n", ConnectMode);
}

void SetBluetoothInquireMode(InquireMode_t InquireMode, uint8_t MaximumBluetoothResponses, uint8_t MaximumResponseTime)
{
	uint8_t timeout = MaximumResponseTime / 1.28;
	if(timeout < 1)
		timeout = 0;
	else if(timeout > 48)
		timeout = 48;

	sendAT_Command("AI+INQM=%d,%d,%d\r\n", InquireMode, MaximumBluetoothResponses, timeout);
}

/**
 * @brief: Pairs with a Bluetooth device with a specified MAC address.
 * @param:
 * 		NonSignificantAddress: 2Bytes
 * 		UpperAddress:          1Bytes
 * 		LowerAddress:          3Bytes
 * @note:
 * 		Bluetooth addresses are usually represented in two ways:
 * 		(NAP: NonSignificant Address Part, UAP: Upper Address Part, LAP: Lower Address Part)
 * 		1. NAP0:NAP1:UAP:LAP0:LAP1:LAP2
 * 		2. NAP:UAP:LAP
 *
 * 		For example, 2020:4:30495 is the address in NAP:UAP:LAP format, means:
 * 			NonSignificantAddress = 0x2020
 * 			UpperAddress          = 0x04
 * 			LowerAddress          = 0x030495
 *
 * 		And 20:20:04:03:04:95 is the address in NAP0:NAP1:UAP:LAP0:LAP1:LAP2 format, means:
 * 			NonSignificantAddress = 0x2020
 * 			UpperAddress          = 0x04
 * 			LowerAddress          = 0x030495
 *
 * 		Then you can use `PairNewBluetoothDevice(0x2020, 0x04, 0x030495, 20);` to pair this device.
 */
void PairNewBluetoothDevice(uint16_t NonSignificantAddress, uint8_t UpperAddress, uint32_t LowerAddress, uint8_t TimeOut)
{
	sendAT_Command("AT+PAIR=%X,%X,%X\r\n", NonSignificantAddress, UpperAddress, LowerAddress);
}

/**
 * @brief: Link to a Bluetooth device with a specified MAC address.
 * @param:
 * 		NonSignificantAddress: 2Bytes
 * 		UpperAddress:          1Bytes
 * 		LowerAddress:          3Bytes
 */
void LinkToBluetoothDevice(uint16_t NonSignificantAddress, uint8_t UpperAddress, uint32_t LowerAddress)
{
	sendAT_Command("AT+LINK=%X,%X,%X\r\n", NonSignificantAddress, UpperAddress, LowerAddress);
}
