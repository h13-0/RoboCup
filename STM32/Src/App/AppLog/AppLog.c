/*
 * Log.c
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#include "AppLog.h"
#include "RobotConfigs.h"

#include <stdio.h>
#include <stdarg.h>

#include "JustFloat.h"
#include "ports.h"

#ifdef __GNUC__
int __io_putchar(int ch) {
	while (LL_USART_IsActiveFlag_TC(DebugPort) == RESET);
	LL_USART_TransmitData8(DebugPort, (uint8_t) ch);
	return ch;
}
#else
int fputc(int ch, FILE *stream)
{
	while(LL_USART_IsActiveFlag_TC(DebugPort) == RESET);
	LL_USART_TransmitData8(DebugPort, (uint8_t)ch);
	return ch;
}
#endif

/**
 * @brief: Init Log Method and Ports.
 */
inline void LogInit()
{
#ifndef DEBUG
	//Release Init Functions.
	SerialInit(DebugPort, DebugPortBaudRate);
#else
	//Debug Init Functions.
	SerialInit(DebugPort, DebugPortBaudRate);
#endif
}

/**
 * @brief: Output Log To Display(Release) or USART(Debug).
 * @param: Log Level and Log in string.
 */
//void mLog(LogLevel_t level, char *fmt, ...)
void __mLog(LogLevel_t level, const char *FileName, int LineNumber, char *fmt, ...)
{
#ifndef DEBUG
	//Release Log Functions.
	switch (level) {
		case Debug:
			return;

		case Info:
			printf("[Info]: ");
			break;

		case Warning:
			printf("[Warning]: ");
			break;

		case Error:
			printf("[Error]: ");
			break;

		case Fatal:
			printf("[Fatal]: ");
			break;

		default:
			return;
	}
#else
	//Debug Log Functions.
	switch (level) {
		case Debug:
			printf("[Debug]: ");
			break;

		case Info:
			printf("[Info]: ");
			break;

		case Warning:
			printf("[Warning]: ");
			break;

		case Error:
			printf("[Error]: ");
			break;

		case Fatal:
			printf("[Fatal]: ");
			break;

		default:
			return;
	}
#endif

	printf("%ldms : %s, %d: \r\n", GetCurrentTimeMillisecond(), FileName, LineNumber);
	printf("    ");

	va_list arg;
	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
	printf("\r\n\r\n");
}

/**
 * @brief: Output Float Data to VOFA+
 * @param: data and length.
 */
void __logJustFloat(uint8_t Length, ...)
{
	va_list args;
	va_start(args, Length);

	if(Length)
	{
		float data[Length];

		for(uint8_t index = 0; index < Length; index++)
		{
			data[index + 1] = va_arg(args, double);
		}

		SendJustFloatFrame(data, Length);
	}

	va_end(args);
}
