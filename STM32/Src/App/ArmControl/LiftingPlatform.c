/*
 * LiftingPlatform.c
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */
#include "RobotConfigs.h"

#if(ArmType == LiftingPlatform)
#include <math.h>

#include "ArmControl.h"
#include "PID.h"

#include "Stepper.h"
#include "Servo.h"

static Stepper_t zAxisStepper = Z_AxisStepper, axisLengthStepper = AL_AxisStepper;

#ifdef DEBUG
PositionPID_t X_AxisPID = { 0 };
PositionPID_t Y_AxisPID = { 0 };
#define xAxisPID X_AxisPID
#define yAxisPID Y_AxisPID
#else
static PositionPID_t xAxisPID = { 0 };
static PositionPID_t yAxisPID = { 0 };
#endif

static uint16_t currentRotateAngle = 0;
static uint16_t currentAxisLength = 0;
static uint16_t currentZ_AxisHeight = 0;

#define PI      (3.1415926)

/**
 * @brief: Init arm control of the robot.
 */
void ArmControlInit(void)
{
	StepperInit(&zAxisStepper);
	StepperInit(&axisLengthStepper);
	while(!Z_AxisZeroingLogicExpression)
	{
		StepperForward(&zAxisStepper, Z_AxisZeroingStep);
		SleepMillisecond(Z_AxisZeroingDelayPerStep);
	}
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
	//Simple logical judgment.
	if((Z_AxisHeight > Z_AxisMaximumHeight + ArmNode3_Length)
			|| (Z_AxisHeight < Z_AxisMinimumHeight - ArmNode3_Length)
			|| (AxialLength > AL_AxisMaximumLength + ArmNode3_Length + ArmNode2_Length)
			|| (AxialLength < AL_AxisMinimumLength + ArmNode3_Length)
			|| (RotationAngle > ArmRotationServoMaximumRotationAngle)
			|| (RotationAngle < 0))
	{
		return TooFar;
	} else
	{
		/**
		 * @group: final control parameters.
		 */
		uint32_t zAxisTargetSteps = 0;
		uint32_t alAxisTargetSteps = 0;
		float armElongationAngle = 0;
		float armParallelAngle = 0;

		uint16_t temp = 0;

		//TODO: Optimize AL-Axis elongation priority.
		if(AxialLength <= AL_AxisMaximumLength + ArmNode3_Length)
		{
			//Move AL-Axis stepper only.
			alAxisTargetSteps = (AxialLength - AL_AxisMinimumLength - ArmNode3_Length) * AL_AxisStepsPerMillimeter;
			armElongationAngle = 0;
			armParallelAngle = 0;
			zAxisTargetSteps = (Z_AxisHeight - Z_AxisMinimumHeight + ArmNode2_Length) * Z_AxisStepsPerMillimeter;
		} else {
			//Move AL-Axis stepper and servos.
			alAxisTargetSteps = (AL_AxisMaximumLength - AL_AxisMinimumLength) * AL_AxisStepsPerMillimeter;
			uint16_t residualElongation = AxialLength - ArmNode3_Length - AL_AxisMaximumLength;
			temp = residualElongation;
			armElongationAngle = atan((double)residualElongation / ArmNode2_Length) * 180 / PI;
			armParallelAngle = armElongationAngle;
			zAxisTargetSteps = (Z_AxisHeight - Z_AxisMinimumHeight + sqrt(pow((double)ArmNode2_Length, 2) - pow((double)residualElongation, 2))) * Z_AxisStepsPerMillimeter;
		}

		//Implement.
		//TODO: Parameter judgment before execution.
		currentRotateAngle = RotationAngle;
		currentAxisLength = AxialLength;
		currentZ_AxisHeight = Z_AxisHeight;

		SetStepperSteps(&axisLengthStepper, alAxisTargetSteps);
		SetStepperSteps(&zAxisStepper, zAxisTargetSteps);
		SetArmNodeAngle(ArmRotation, currentRotateAngle);
		SetArmNodeAngle(ArmElongation, armElongationAngle);
		SetArmNodeAngle(ArmParallel, armParallelAngle);

		float data[] = { (zAxisTargetSteps / Z_AxisStepsPerMillimeter), (alAxisTargetSteps / AL_AxisStepsPerMillimeter), temp, RotationAngle, armElongationAngle };
		LogJustFloat(data, 5);
	}
	return ArmControlOK;
}

/**
 * @brief: Get claw position in **Polar coordinates** in Open loop control system.
 * @param: Pointer of parameters.
 */
void GetOpenLoopClawPosition(uint16_t *RotationAngle, uint16_t *AxialLength, uint16_t *Z_AxisHeight)
{
	*RotationAngle = currentRotateAngle;
	*AxialLength = currentAxisLength;
	*Z_AxisHeight = currentZ_AxisHeight;
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
