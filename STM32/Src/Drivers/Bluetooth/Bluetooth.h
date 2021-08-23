/*
 * Bluetooth.h
 *
 *  Created on: 2021Äê8ÔÂ23ÈÕ
 *      Author: h13
 *
 * @note: This library is **only** available for version 3.0 of HC-05
 */

#ifndef DRIVERS_BLUETOOTH_BLUETOOTH_H_
#define DRIVERS_BLUETOOTH_BLUETOOTH_H_

#include <stdint.h>

/**
 * @brief:   Send message to bluetooth.
 * @example:
 * 		BluetoothPrintf("Hello, world!\r\n");
 * 		BluetoothPrintf("pi=%f\r\n", 3.1415926);
 */
void BluetoothPrintf(char *fmt, ...);

void BluetoothSendChar(char Data);

void ResetBluetoothConfigs(void);

typedef enum
{
	HC05_2400bps,
	HC05_4800bps,
	HC05_9600bps,
	HC05_19200bps,
	HC05_38400bps,
	HC05_57600bps,
	HC05_115200bps,
	HC05_230400bps,
	HC05_460800bps,
	HC05_921600bps,
	HC05_1382400bps,
} HC05_BaundRate_t;

typedef enum
{
	Stop_1_Bit = 0,
	Stop_2_Bits = 1,
} HC05_StopBit_t;

typedef enum
{
	NoneCheckBit = 0,
	OddCheckBit = 1,
	EvenCheckBit = 2,
} HC05_CheckBit_t;

void SetBluetoothBaundRate(HC05_BaundRate_t BaundRate, HC05_StopBit_t StopBit, HC05_CheckBit_t CheckBit);

/**
 * @brief: Set pairing code.
 * @param: Code in 1 to 8 decimal digits.
 */
void SetBluetoothPin(char *Pin);

typedef enum
{
	Slave = 0,
	Master = 1,
	SlaveLoop = 2,
} ROLE_t;

void SetBluetoothRole(ROLE_t Role);

/**
 * @brief:
 * @TODO:  To adapt hexadecimal and adecimal parameters.
 */
void SetBluetoothClass(uint32_t Class);

typedef enum
{
	SpecifiedAddress = 0,
	AnyAddress = 1,
	SlaveLoopAddress = 2,
} CMODE_t;

void SetBluetoothConnectMode(CMODE_t ConnectMode);

typedef enum
{
	InquireModeStandard = 0,
	InquireModeRSSI     = 1,
} InquireMode_t;

void SetBluetoothInquireMode(InquireMode_t InquireMode, uint8_t MaximumBluetoothResponses, uint8_t MaximumResponseTime);

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
void PairNewBluetoothDevice(uint16_t NonSignificantAddress, uint8_t UpperAddress, uint32_t LowerAddress, uint8_t TimeOut);

/**
 * @brief: Link to a Bluetooth device with a specified MAC address.
 * @param:
 * 		NonSignificantAddress: 2Bytes
 * 		UpperAddress:          1Bytes
 * 		LowerAddress:          3Bytes
 */
void LinkToBluetoothDevice(uint16_t NonSignificantAddress, uint8_t UpperAddress, uint32_t LowerAddress);

#endif /* DRIVERS_BLUETOOTH_BLUETOOTH_H_ */
