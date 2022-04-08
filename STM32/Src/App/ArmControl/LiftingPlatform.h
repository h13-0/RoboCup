/*
 * LiftingPlatform.h
 *
 *  Created on: 2022Äê4ÔÂ2ÈÕ
 *      Author: h13
 */

#ifndef APP_ARMCONTROL_LIFTINGPLATFORM_H_
#define APP_ARMCONTROL_LIFTINGPLATFORM_H_

#include "RobotConfigs.h"

#if(ArmType == LiftingPlatform)
#include <stdint.h>
#include "ports.h"
/**
 * @brief: Init arm control of the robot.
 * @note: This function requires override in the corresponding sub file.
 */
void ArmControlInit(void);

typedef enum
{
	OpenLoopMode,
	//CloseLoopMode, //Not implemented yet.
	ManualMode,
} ArmControlMode_t;


/**
 * @brief: Switch Arm control mode.
 * @param: Target mode.
 * @note:
 * 		**Please retract servos manually before switching.**
 */
void SwitchArmControlMode(ArmControlMode_t Mode);

// Commen apis.
/**
 * @brief: Closure the claw.
 * @TODO:  Use current to judge whether it is firmly grasped.
 */
void ClosureClaw(void);

/**
 * @brief: Release the claw.
 */
void ReleaseClaw(void);

// OpenLoopMode apis.
typedef enum
{
	ArmControlOK = 0,          //Control success.
	ControlModeError,          //Control mode error.
	CoordinatesTooFar,         //
	CoordinatesValueIsInvalid, //

	//
	AimTargetInvalid,          //
	AimingTimeout,             //
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
ArmControlResult_t GetOpenLoopClawPosition(uint16_t *RotationAngle, uint16_t *AxialLength, uint16_t *Z_AxisHeight);

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
	MoveZ_AxisHeight
} TargetCoordinates_t;

ArmControlResult_t SmoothMoveTo(TargetCoordinates_t TargetCoordinates, uint16_t TargetValue, mtime_t MillisecondDelayPerStep);

typedef enum
{
	AimApple,
	AimTarget,
	FruitFocus
} Target_t;

/**
 * @brief: Aim the mechanical claw at Target.
 * @param:
 * 		Target_t Target:  Target in enum.
 * 		mtime_t  TimeOut: Time out in millisecond.
 * @note:
 * 		When using open-loop control, the `RelativeZ_AxisHeight` does not take effect.
 */
ArmControlResult_t AimAt(Target_t Target, mtime_t TimeOut);

// ManualMode apis.
ArmControlResult_t SetZ_AxisHeight(uint16_t Heigth);

ArmControlResult_t SetAL_AxisLength(uint16_t Legnth);

uint16_t GetZ_AxisHeight(void);

uint16_t GetAL_AxisHeight(void);

uint8_t IsZ_AxisBusy(void);

#endif

#endif /* APP_ARMCONTROL_LIFTINGPLATFORM_H_ */
