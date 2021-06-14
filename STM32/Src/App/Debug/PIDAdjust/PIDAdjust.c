/*
 * PIDAdjust.c
 *
 *  Created on: 2021Äê6ÔÂ10ÈÕ
 *      Author: h13
 */

#ifdef DEBUG
#include "PIDAdjust.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "PID.h"
#include "FastMatch.h"

#define process

extern PositionPID_t AnglePID;
extern PositionPID_t ForwardPID;

/**
 * @brief: PID Adjust Handler.
 * @param:
 * 		char    *data: pointer of string.
 * 		uint8_t len:   length of string.
 * @usage:
 *		${Key}:${hex of float}\r\n
 */
__attribute__((always_inline)) inline void PIDAdjustHandler(char *data, uint8_t len)
{
	matchKeyFloat(data, len, "AP:", 3, AnglePID.proportion);

	matchKeyFloat(data, len, "AI:", 3, AnglePID.integral);

	matchKeyFloat(data, len, "AD:", 3, AnglePID.derivative);

	matchKeyFloat(data, len, "TA:", 3, AnglePID.setpoint);
}

#endif
