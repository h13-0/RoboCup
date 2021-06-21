/*
 * Log.h
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#ifndef APPLOG_APPLOG_H_
#define APPLOG_APPLOG_H_

#include <stdint.h>

typedef enum
{
	Debug,
	Info,
	Warning,
	Error,
	Fatal
}LogLevel_t;

/**
 * @brief: Init Log Method and Ports.
 */
void LogInit();

/**
 * @brief: Output Log To Display(Release) or USART(Debug).
 * @param: Log Level and Log in format.
 */
void Log(LogLevel_t level, char *fmt, ...);

/**
 * @brief: Output Float Data to VOFA+
 * @param: data and length.
 */
void LogJustFloat(float data[], uint8_t len);

#endif /* LOG_LOG_H_ */
