/*
 * MotionControl.h
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */

#ifndef APP_MOTIONCONTROL_MOTIONCONTROL_H_
#define APP_MOTIONCONTROL_MOTIONCONTROL_H_

#include <stdint.h>
#include <stdnoreturn.h>

typedef enum
{
	Left,
	Right,
	BackWard
} direction_t;

/**
 * @brief: Init Motion Control.
 * @note:
 * 		Before you can use this, you need to initialize Motor and MPU manually ahead of time.
 * 		It will Init Direction PID, turn to zero and brake.
 */
void MotionControlInit(void);

/**
 * @brief: Turn To ..
 * @param: Direction you want to turn to.
 */
void TurnTo(direction_t Direction);

#ifdef DEBUG
/**
 * @brief: Keep Angle To Adjust PID Value.
 */
_Noreturn void KeepAngle(void);

_Noreturn void KeepDistance(uint16_t Distance);

#endif

/**
 *
 */
void StraightUntill(uint16_t distance);

/**
 * @group: PID Calculate Handler.
 */
void DirectionPIDCalculateHandler(void);

void ForwardPIDCalculateHandler(void);

#endif /* APP_MOTIONCONTROL_MOTIONCONTROL_H_ */
