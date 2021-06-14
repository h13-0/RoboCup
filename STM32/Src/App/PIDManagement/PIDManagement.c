/*
 * PIDRealize.c
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */
#include "PIDManagement.h"

/**
 * @group: PID Handlers
 */
#include "MotionControl.h"

#include <stdint.h>

/**
 * @brief: PID Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
__attribute__((always_inline)) inline void PIDHandler(void) {
	DirectionPIDCalculateHandler();
	ForwardPIDCalculateHandler();
}
