/*
 * Log.h
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#ifndef APPLOG_APPLOG_H_
#define APPLOG_APPLOG_H_

#include <stdint.h>

#ifndef COUNT_ARGS
#define COUNT_ARGS(X...) __COUNT_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __COUNT_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#endif

typedef enum
{
	Debug,
	Info,
	Warning,
	Error,
	Fatal
} LogLevel_t;

/**
 * @brief: Init Log Method and Ports.
 */
void LogInit();

/**
 * @brief: Output Log To Display(Release) or USART(Debug).
 * @param: Log Level and Log in format.
 */
#define Log(level, fmt, ...) __mLog(level, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
void __mLog(LogLevel_t level, const char *FileName, int LineNumber, char *fmt, ...);

/**
 * @brief: Output Float Data to VOFA+
 * @param: data list.
 * @usage:
 * 		LogJustFloat(Value1);
 * 		LogJustFloat(Value1, Value2);
 * 		...
 * @note:
 * 		The number of parameters should be less than 14.
 */
#define LogJustFloat(...) __logJustFloat(COUNT_ARGS(X ##__VA_ARGS__), ##__VA_ARGS__)
void __logJustFloat(uint8_t Length, ...);

#endif /* LOG_LOG_H_ */
