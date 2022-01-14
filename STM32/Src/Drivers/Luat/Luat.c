/*
 * Luat.c
 *
 *  Created on: 2021Äê8ÔÂ18ÈÕ
 *      Author: h13
 */
#include "Luat.h"
#include "RobotConfigs.h"
#include "ports.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * @brief: Init peripheral for Luat.
 */
void LuatModuleInit(void)
{
	SerialInit(LuatPort, LuatPortBaudRate);
}

/**
 * @brief: Create new message for Luat.
 * @usage:
 * 		char messageBuffer[128] = { 0 };
 * 		LuatMessage_t message = LuatCreatNewMessage(messageBuffer, 128);
 */
LuatMessage_t LuatCreatNewMessage(char *BufferPtr, uint8_t BufferSize)
{
	memset(BufferPtr, 0, BufferSize);
	return (LuatMessage_t){.Index = 0, .BufferSize = BufferSize, .BufferPtr = BufferPtr};
}

/**
 * @brief: Add one character to message.
 * @param:
 * 		Msg:       LuatMessage_t, The Appended target.
 * 		Character: Unicode_t, The characters you want to add.
 */
int LuatAddMessageContent(LuatMessage_t *Msg, Unicode_t Character)
{
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	memcpy(Msg -> BufferPtr + Msg -> Index, &Character, 2);
	Msg -> Index += 2;
#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
	*(Msg -> BufferPtr + Msg -> Index) = Character >> 8;
	Msg -> Index ++;
	*(Msg -> BufferPtr + Msg -> Index) = Character & 0xff;
	Msg -> Index ++;
#endif
	return Msg -> BufferSize - Msg -> Index;
}

int __luatAddMessageContents(LuatMessage_t *Msg, uint8_t Length, ...)
{
	va_list args;
	va_start(args, Length);

	for(int i = 0; i < Length; i++)
	{
		LuatAddMessageContent(Msg, va_arg(args, int));
	}

	va_end(args);
	return 0;
}

static void sendAT_Command(char *fmt, ...)
{
	uint8_t length;
	char buffer[48];
	va_list arg;

	SleepMillisecond(10);

	va_start(arg, fmt);
	length = vsprintf(buffer, fmt, arg);
	va_end(arg);

	for(uint8_t i = 0; i < length; i++)
	{
		SerialSend(LuatPort, buffer[i]);
	}

	SleepMillisecond(300);
}

/**
 * @brief: Send message to target phone.
 * @param:
 * 		LuatMessage_t *Msg:            The message you want to send.
 * 		const char* TargetPhoneNumber: Target Phone Number.
 * @note:
 * 		The phone number is a string, not an integer.
 * 		Such as 0371-66667777
 */
void LuatSendMessage(LuatMessage_t *Msg, const char* TargetPhoneNumber)
{
	sendAT_Command("AT+CMGF=1\r\n");
	sendAT_Command("AT+CSMP=17,167,0,8\r\n");
	sendAT_Command("AT+CMGS=\"%s\"\r\n", TargetPhoneNumber);

	for(uint8_t index = 0; index < Msg -> Index; index++)
	{
		SerialSend(LuatPort, *(Msg -> BufferPtr + index));
	}

	SerialSend(LuatPort, 0x1a);
}
