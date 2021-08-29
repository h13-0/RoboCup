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
 * @brief: Set claw position in **Polar coordinates** in Open loop control system.
 * @param:
 * 		RotationAngle: The Angle of rotation relative to the zero position. Range: [0, 180]
 * 		AxialLength:   The length of the axis relative to zero in millimeters.
 * 		Z_AxisHeight:  The height above the ground in millimeters.
 */
ArmControlResult_t SetOpenLoopClawPosition(uint16_t RotationAngle, uint16_t AxialLength, uint16_t Z_AxisHeight);

/**
 * @brief: Get claw position in **Polar coordinates** in Open loop control system.
 * @param: Pointer of parameters.
 */
void GetOpenLoopClawPosition(uint16_t *RotationAngle, uint16_t *AxialLength, uint16_t *Z_AxisHeight);

typedef enum
{
	Apple,
	Target,
} Target_t;

/**
 * @brief: Aim the mechanical claw at Target.
 * @param:
 * 		Target_t AimTarget:            Target in enum.
 * 		mtime_t  TimeOut:              Time out in millisecond.
 * @note:
 * 		When using open-loop control, the `RelativeZ_AxisHeight` does not take effect.
 */
void AimAt(Target_t AimTarget, mtime_t TimeOut);

#endif /* APP_ARMCONTROL_ARMCONTROL_H_ */
