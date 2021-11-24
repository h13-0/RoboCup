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
#include "Drivers.h"
#include "PID.h"

#include "Stepper.h"
#include "Servo.h"

#define PI                (3.1415926)

static Stepper_t zAxisStepper = Z_AxisStepper, axisLengthStepper = AL_AxisStepper;
static GPIO_t zLimitSensorIO = Z_LimitSensorIO;

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
static float xAxisPID_Accumulator = 0;
static float yAxisPID_Accumulator = 0;

/**
 * @brief: Init arm control of the robot.
 */
void ArmControlInit(void)
{
	StepperInit(&zAxisStepper);
	StepperInit(&axisLengthStepper);

	GPIO_Init(&zLimitSensorIO, Input);

	//Zeroing.
	while(!Z_AxisZeroingLogicExpression)
	{
		StepperForward(&zAxisStepper, Z_AxisZeroingStep);
		SleepMillisecond(Z_AxisZeroingDelayPerStep);
	}

	currentZ_AxisHeight = Z_AxisZeroPoint;
	zAxisStepper.CurrentSteps = 0;
	zAxisStepper.TargetSteps = 0;

	while(!AL_AxisZeroingLogicExpression)
	{
		StepperForward(&axisLengthStepper, AL_AxisZeroingStep);
		SleepMillisecond(AL_AxisZeroingDelayPerStep);
	}

	currentAxisLength = AL_AxisZeroPoint;
	axisLengthStepper.CurrentSteps = 0;
	axisLengthStepper.TargetSteps = 0;

	xAxisPID.proportion   = X_AxisPID_Proportion;
	xAxisPID.integration  = X_AxisPID_Integration;
	xAxisPID.differention = 0.0;   //note: Try not to use differention.

	xAxisPID.configs.autoResetIntegration = disable;
	xAxisPID.configs.limitIntegration = enable;

	xAxisPID.maxAbsOutput = ArmRotationServoMaximumRotationAngle / 75.0;
	xAxisPID.maximumAbsValueOfIntegrationOutput = xAxisPID.maxAbsOutput * 0.1;

	yAxisPID.proportion   = Y_AxisPID_Proportion;
	yAxisPID.integration  = Y_AxisPID_Integration;
	yAxisPID.differention = 0.0;

	yAxisPID.configs.autoResetIntegration = disable;
	yAxisPID.configs.limitIntegration = enable;

	yAxisPID.maxAbsOutput = (MaximumAxialLength) / 75.0;
	yAxisPID.maximumAbsValueOfIntegrationOutput = yAxisPID.maxAbsOutput * 0.1;
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
		int32_t zAxisTargetSteps = 0;
		int32_t alAxisTargetSteps = 0;
		float armElongationAngle = 0;
		float armParallelAngle = 0;

		//TODO: Optimize AL-Axis elongation priority.
		if(AxialLength <= AL_AxisMaximumLength + ArmNode3_Length)
		{
			//Move AL-Axis stepper only.
			alAxisTargetSteps = (AxialLength - AL_AxisMinimumLength - ArmNode3_Length) * AL_AxisStepsPerMillimeter;
			armElongationAngle = 0;
			armParallelAngle = 0;
			zAxisTargetSteps = (Z_AxisHeight - Z_AxisZeroPoint) * Z_AxisStepsPerMillimeter;
		} else {
			//Move AL-Axis stepper and servos.
			alAxisTargetSteps = (AL_AxisMaximumLength - AL_AxisMinimumLength) * AL_AxisStepsPerMillimeter;
			uint16_t residualElongation = AxialLength - ArmNode3_Length - AL_AxisMaximumLength;
			armElongationAngle = asin((double)residualElongation / ArmNode2_Length) * 180 / PI;
			armParallelAngle = armElongationAngle;
			zAxisTargetSteps = ( (Z_AxisHeight + sqrt(pow((double)ArmNode2_Length, 2) - pow((double)residualElongation, 2))) - (Z_AxisZeroPoint + ArmNode2_Length)) * Z_AxisStepsPerMillimeter;
		}

		//Implement.
		//TODO: Parameter judgment before execution.
		if((zAxisTargetSteps / Z_AxisStepsPerMillimeter) > 0)
		{
			return TooFar;
		}

		if((zAxisTargetSteps / Z_AxisStepsPerMillimeter) < Z_AxisMinimumHeight - Z_AxisMaximumHeight)
		{
			return TooFar;
		}

		if(armElongationAngle > MaximumElongationAngle)
		{
			return TooFar;
		}

		currentRotateAngle = RotationAngle;
		currentAxisLength = AxialLength;
		currentZ_AxisHeight = Z_AxisHeight;

		SetStepperSteps(&axisLengthStepper, alAxisTargetSteps);
		SetStepperSteps(&zAxisStepper, zAxisTargetSteps);
		SetArmNodeAngle(ArmRotation, currentRotateAngle);
		SetArmNodeAngle(ArmElongation, armElongationAngle);
		SetArmNodeAngle(ArmParallel, armParallelAngle);
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

static void calculateX_AxisPID(float CurrentTargetX, float AimCenterX)
{
	xAxisPID.setpoint = AimCenterX;
	xAxisPID_Accumulator += PosPID_Calc(&xAxisPID, CurrentTargetX);
	if(xAxisPID_Accumulator > ArmRotationServoMaximumRotationAngle)
	{
		xAxisPID_Accumulator = ArmRotationServoMaximumRotationAngle;
	} else if(xAxisPID_Accumulator < 0)
	{
		xAxisPID_Accumulator = 0;
	}
}

static void calculateY_AxisPID(float CurrentTargetY, float AimCenterY)
{
	yAxisPID.setpoint = AimCenterY;
	yAxisPID_Accumulator += PosPID_Calc(&yAxisPID, CurrentTargetY);
	if(yAxisPID_Accumulator > MaximumAxialLength)
	{
		yAxisPID_Accumulator = MaximumAxialLength;
	} else if(yAxisPID_Accumulator < MinimumAxialLength)
	{
		yAxisPID_Accumulator = MinimumAxialLength;
	}
}


/**
 * @brief: Aim the mechanical claw at Target.
 * @param:
 * 		Target_t Target:  Target in enum.
 * 		mtime_t  TimeOut: Time out in millisecond.
 * @note:
 * 		When using open-loop control, the `RelativeZ_AxisHeight` does not take effect.
 */
void AimAt(Target_t Target, mtime_t TimeOut)
{
	mtime_t lastCalculateTime = 0;
	mtime_t startTime = GetCurrentTimeMillisecond();
	mtime_t startStableTime = GetCurrentTimeMillisecond();
	xAxisPID_Accumulator = currentRotateAngle;
	yAxisPID_Accumulator = currentAxisLength;

	while(1)
	{
		Coordinates_t coordinates = { 0 };
		float currentX = 0;
		float currentY = 0;

		switch(Target)
		{
		case AimApple:
			GetAppleCoordinates(&coordinates);
			if((GetCurrentTimeMillisecond() - coordinates.TimeStamp < (1000.0 * MaximumFPS_Fluctuation / AppleDetectionAverageFPS))
					&& (GetCurrentTimeMillisecond() - lastCalculateTime > (1000.0 * MaximumFPS_Fluctuation / AppleDetectionAverageFPS)))
			{
				currentX = coordinates.X;
				currentY = coordinates.Y;
				if(fabs(currentX - AppleAimCenterX) > AimToleranceErrorX)
				{
					calculateX_AxisPID(currentX, AppleAimCenterX);
					startStableTime = GetCurrentTimeMillisecond();
				}

				if(fabs(currentY - AppleAimCenterY) > AimToleranceErrorY)
				{
					calculateY_AxisPID(currentY, AppleAimCenterY);
					startStableTime = GetCurrentTimeMillisecond();
				}

				SetOpenLoopClawPosition(xAxisPID_Accumulator, yAxisPID_Accumulator, currentZ_AxisHeight);
				lastCalculateTime = GetCurrentTimeMillisecond();
			} else if(GetCurrentTimeMillisecond() - coordinates.TimeStamp > (1000.0 * MaximumFPS_Fluctuation / AppleDetectionAverageFPS)) {
				startStableTime = GetCurrentTimeMillisecond();
			}
			break;

		case AimTarget:
			GetTargetCoordinates(&coordinates);
			if((GetCurrentTimeMillisecond() - coordinates.TimeStamp < (1000.0 * MaximumFPS_Fluctuation / TargetDetectionAverageFPS))
				&& (GetCurrentTimeMillisecond() - lastCalculateTime > (1000.0 * MaximumFPS_Fluctuation / TargetDetectionAverageFPS)))
			{
				currentX = coordinates.X;
				currentY = coordinates.Y;
				if(fabs(currentX - TargetAimCenterX) > AimToleranceErrorX)
				{
					calculateX_AxisPID(currentX, TargetAimCenterX);
					startStableTime = GetCurrentTimeMillisecond();
				}

				if(fabs(currentY - TargetAimCenterY) > AimToleranceErrorY)
				{
					calculateY_AxisPID(currentY, TargetAimCenterY);
					startStableTime = GetCurrentTimeMillisecond();
				}

				SetOpenLoopClawPosition(xAxisPID_Accumulator, yAxisPID_Accumulator, currentZ_AxisHeight);
				lastCalculateTime = GetCurrentTimeMillisecond();
			} else if(GetCurrentTimeMillisecond() - coordinates.TimeStamp > (1000.0 * MaximumFPS_Fluctuation / TargetDetectionAverageFPS)) {
				startStableTime = GetCurrentTimeMillisecond();
			}
			break;

		default:
			return;
		}

		if(GetCurrentTimeMillisecond() - startStableTime > ClawStableTimesLimit)
		{
			xAxisPID._sumError = 0;
			yAxisPID._sumError = 0;
			return;
		}

		if(GetCurrentTimeMillisecond() - startTime > TimeOut)
		{
			xAxisPID._sumError = 0;
			yAxisPID._sumError = 0;
			return;
		}

		float data[] = { coordinates.X, TargetAimCenterX, coordinates.Y, TargetAimCenterY, coordinates.TimeStamp, startStableTime };
		LogJustFloat(data, 6);
	}
}

#endif
