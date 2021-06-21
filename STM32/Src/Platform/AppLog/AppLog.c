/*
 * Log.c
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#include "AppLog.h"
#include <stdarg.h>
#include "SerialPrintf.h"
//Serial Printf Lib
#include "JustFloat.h"


/**
 * @brief: Init Log Method and Ports.
 */
inline void LogInit()
{
#ifndef DEBUG
	//Release Init Functions.

#else
	//Debug Init Functions.
	SetSerialPrintfTarget(USART1);
	SetJustFloatSerialPort(USART1);
#endif
}

/**
 * @brief: Output Log To Display(Release) or USART(Debug).
 * @param: Log Level and Log in string.
 */
void Log(LogLevel_t level, char *fmt, ...)
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

	va_list arg;
	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
}

/**
 * @brief: Output Float Data to VOFA+
 * @param: data and length.
 */
inline __attribute__((always_inline)) void LogJustFloat(float data[], uint8_t len)
{
	SendJustFloatFrame(data, len);
}
