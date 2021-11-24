/*
 * ArmControl.h
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */

#ifndef APP_ARMCONTROL_ARMCONTROL_H_
#define APP_ARMCONTROL_ARMCONTROL_H_

#include <stdint.h>
#include "ports.h"

/**
 * @brief: Init arm control of the robot.
 * @note: This function requires override in the corresponding sub file.
 */
void ArmControlInit(void);

/**
 * @brief: Closure the claw.
 * @TODO:  Use current to judge whether it is firmly grasped.
 */
void ClosureClaw(void);

/**
 * @brief: Release the claw.
 */
void ReleaseClaw(void);

typedef enum
{
	ArmControlOK = 0,
	TooFar,
	TargetValueIsInvalid,
} ArmControlResult_t;

/**
 * @brief: Set claw position in **Cylindrical coordinates** in Open loop control system.
 * @param:
 * 		RotationAngle: The Angle of rotation relative to the zero position. Range: [0, 180]
 * 		AxialLength:   The length of the axis relative to zero in millimeters.
 * 		Z_AxisHeight:  The height above the ground in millimeters.
 *
 * @note:
 * 		This function requires override in the corresponding sub file.
 */
ArmControlResult_t SetOpenLoopClawPosition(uint16_t RotationAngle, uint16_t AxialLength, uint16_t Z_AxisHeight);

/**
 * @brief: Get claw position in **Cylindrical coordinates** in Open loop control system.
 * @param: Pointer of parameters.
 */
void GetOpenLoopClawPosition(uint16_t *RotationAngle, uint16_t *AxialLength, uint16_t *Z_AxisHeight);

/**
 * @brief: Smooth move claw to position in **Cylindrical coordinates** in Open loop control system.
 * @param:
 * 		TargetCoordinates:       Target control coordinates.
 * 		TargetValue:             Target value.
 * 		MillisecondDelayPerStep: Milliseconds per mm / degree delay.
 */
typedef enum
{
	MoveRotationAngle,
	MoveAxialLength,
	MoveZ_AxisHeight,
} TargetCoordinates_t;

void SmoothMoveTo(TargetCoordinates_t TargetCoordinates, uint16_t TargetValue, mtime_t MillisecondDelayPerStep);

typedef enum
{
	AimApple,
	AimTarget,
} Target_t;

/**
 * @brief: Aim the mechanical claw at Target.
 * @param:
 * 		Target_t Target:  Target in enum.
 * 		mtime_t  TimeOut: Time out in millisecond.
 * @note:
 * 		When using open-loop control, the `RelativeZ_AxisHeight` does not take effect.
 */
void AimAt(Target_t Target, mtime_t TimeOut);

#endif /* APP_ARMCONTROL_ARMCONTROL_H_ */
