/*
 * PIDAdjust.h
 *
 *  Created on: 2021Äê6ÔÂ10ÈÕ
 *      Author: h13
 */

#ifndef APP_DEBUG_PARAMETERADJUST_PARAMETERADJUST_H_
#define APP_DEBUG_PARAMETERADJUST_PARAMETERADJUST_H_

#ifdef DEBUG
#include <stdint.h>
/**
 * @brief: PID Adjust Handler.
 * @param:
 * 		char    *data: pointer of string.
 * 		uint8_t len:   length of string.
 * @usage:
 *		${Key}:${hex of float}\r\n
 *
 * @example:
 *
 */
void PIDAdjustHandler(char *data, uint8_t len);

#endif

#endif /* APP_DEBUG_PARAMETERADJUST_PARAMETERADJUST_H_ */
