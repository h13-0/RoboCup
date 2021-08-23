/*
 * BluetoothTest.c
 *
 *  Created on: 2021Äê8ÔÂ23ÈÕ
 *      Author: h13
 */
#include "BluetoothTest.h"
#include "RobotConfigs.h"

#include "Ports.h"
#include "Drivers.h"

void BluetoothInit(void)
{
	while(GetCurrentTimeMillisecond() < 1000);
	SetBluetoothRole(Master);
	SetBluetoothClass(0);
	SetBluetoothInquireMode(InquireModeRSSI, 9, 60);
	SetBluetoothConnectMode(AnyAddress);
	SetBluetoothPin("2020");
	PairNewBluetoothDevice(BluetoothTargetNonSignificantAddress, BluetoothTargetUpperAddress, BluetoothTargetLowerAddress, 20);
}

void ConnectToBluetoothDevice(void)
{
	LinkToBluetoothDevice(BluetoothTargetNonSignificantAddress, BluetoothTargetUpperAddress, BluetoothTargetLowerAddress);
}

void TurnOnTheBluetoothLight(void)
{
	for(uint8_t times = 0; times < 10; times++)
	{
		//BluetoothSendChar('A');
		SendBluetoothMessageContent();
		SleepMillisecond(100);
	}
}
