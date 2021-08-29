/*
 * LiftingPlatform.c
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */
#include "RobotConfigs.h"

#if(ArmType == LiftingPlatform)
#include "ArmControl.h"
#include "PID.h"

#include "Stepper.h"

static Stepper_t zAxisStepper = Z_AxisStepper, axisLengthStepper = { 0 };

#ifdef DEBUG
PositionPID_t X_AxisPID = { 0 };
PositionPID_t Y_AxisPID = { 0 };
#define xAxisPID X_AxisPID
#define yAxisPID Y_AxisPID
#else
static PositionPID_t xAxisPID = { 0 };
static PositionPID_t yAxisPID = { 0 };
#endif

static float currentZ_AxisHeight = 0.0, currentAxisLength = 0.0;

/**
 * @brief: Init arm control of the robot.
 */
void ArmControlInit(void)
{

}

/**
 * @brief: Set claw position in **Polar coordinates** in Open loop control system.
 * @param:
 * 		RotationAngle: The Angle of rotation relative to the zero position. Range: [0, 180]
 * 		AxialLength:   The length of the axis relative to zero in millimeters.
 * 		Z_AxisHeight:  The height above the ground in millimeters.
 */
ArmControlResult_t SetOpenLoopClawPosition(uint16_t RotationAngle, uint16_t AxialLength, uint16_t Z_AxisHeight)
{

	return ArmControlOK;
}

/**
 * @brief: Get claw position in **Polar coordinates** in Open loop control system.
 * @param: Pointer of parameters.
 */
void GetOpenLoopClawPosition(uint16_t *RotationAngle, uint16_t *AxialLength, uint16_t *Z_AxisHeight)
{

}

/**
 * @brief: Aim the mechanical claw at Target.
 * @param:
 * 		Target_t AimTarget:            Target in enum.
 * 		mtime_t  TimeOut:              Time out in millisecond.
 * @note:
 * 		When using open-loop control, the `RelativeZ_AxisHeight` does not take effect.
 */
void AimAt(Target_t AimTarget, mtime_t TimeOut)
{

}

#endif
