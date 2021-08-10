/*
 * ArmControl.h
 *
 *  Created on: 2021��8��7��
 *      Author: h13
 */

#ifndef APP_ARMCONTROL_ARMCONTROL_H_
#define APP_ARMCONTROL_ARMCONTROL_H_
#include "ports.h"

typedef enum
{
	ArmControlOK = 0,
	TooFar,
	TargetValueIsInvalid,
} ArmControlResult_t;

/**
 * @brief: Init arm control of the robot.
 */
void ArmControlInit(void);

/**
 * @brief: Set claw position in **Polar coordinates** in Open loop control system.
 * @param:
 * 		RotationAngle: The Angle of rotation relative to the zero position. Range: [0, 180]
 * 		AxialLength:   The length of the axis relative to zero in millimeters.
 * 		Z_AxisHeight:  The height above the ground in millimeters.
 */
ArmControlResult_t SetOpenLoopClawPosition(float RotationAngle, float AxialLength, float Z_AxisHeight);

/**
 * @brief: Aim the mechanical claw at the apple.
 * @param:
 * 		float AbsoluteZ_AxisHeight:   Absolute Z-Axis height.
 * 		[float RelativeZ_AxisHeight]: Relative Z-Axis height.
 * 		mtime_t TimeOut:              Time out in millisecond.
 * @note:
 * 		When using open-loop control, the `RelativeZ_AxisHeight` does not take effect.
 */
void AimAtApple(float AbsoluteZ_AxisHeight, float RelativeZ_AxisHeight, mtime_t TimeOut);

/**
 * @brief: Grab.
 * @TODO:  Use current to judge whether it is firmly grasped.
 */
void Grab(void);

/**
 * @brief: Arm control PID calculate handler.
 * @note:  **The call frequency must be 50 Hz!!**
 */
void ArmControlPIDCalculateHandler(void);

#endif /* APP_ARMCONTROL_ARMCONTROL_H_ */
