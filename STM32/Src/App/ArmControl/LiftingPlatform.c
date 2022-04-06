/*
 * LiftingPlatform.c
 *
 *  Created on: 2021Äê8ÔÂ25ÈÕ
 *      Author: h13
 */
#include "RobotConfigs.h"

#if(ArmType == LiftingPlatform)
#include "LiftingPlatform.h"

#include <math.h>

#include "ArmControl.h"
#include "Drivers.h"
#include "PID.h"

#include "Stepper.h"
#include "Servo.h"

#define PI                (3.1415926)

static Stepper_t zAxisStepper = Z_AxisStepper, alAxisLengthStepper = AL_AxisStepper;
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

static ArmControlMode_t mode;
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
	StepperInit(&alAxisLengthStepper);

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
		StepperForward(&alAxisLengthStepper, AL_AxisZeroingStep);
		SleepMillisecond(AL_AxisZeroingDelayPerStep);
	}

	currentAxisLength = AL_AxisZeroPoint;
	alAxisLengthStepper.CurrentSteps = 0;
	alAxisLengthStepper.TargetSteps = 0;

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

	SwitchArmControlMode(OpenLoopMode);
}

void SwitchArmControlMode(ArmControlMode_t Mode)
{
	if((mode == OpenLoopMode) && (Mode == ManualMode))
	{
		mode = Mode;
	}
}


/**
 * @brief: Closure the claw.
 * @TODO:  Use current to judge whether it is firmly grasped.
 */
void ClosureClaw(void)
{
	SetArmNodeAngle(ClawGrab, MaximumRotationAngleOfGraspingApple);
}

/**
 * @brief: Release the claw.
 */
void ReleaseClaw(void)
{
	SetArmNodeAngle(ClawGrab, 180);
}

/**
 * @brief: Smooth move claw to position in **Cylindrical coordinates** in Open loop control system.
 * @param:
 * 		TargetCoordinates:       Target control coordinates.
 * 		TargetValue:             Target value.
 * 		MillisecondDelayPerStep: Milliseconds per mm / degree delay.
 */
ArmControlResult_t SmoothMoveTo(TargetCoordinates_t TargetCoordinates, uint16_t TargetValue, mtime_t MillisecondDelayPerStep)
{
	uint16_t rotationAngle = 0, axialLength = 0, zAxisHeight = 0;
	GetOpenLoopClawPosition(&rotationAngle, &axialLength, &zAxisHeight);
	switch (TargetCoordinates) {
		case MoveRotationAngle:
			while(TargetValue != rotationAngle)
			{
				if(rotationAngle < TargetValue)
				{
					rotationAngle ++;
				} else {
					rotationAngle --;
				}
				SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
				SleepMillisecond(MillisecondDelayPerStep);
			}
			break;

		case MoveAxialLength:
			while(TargetValue != axialLength)
			{
				if(axialLength < TargetValue)
				{
					axialLength ++;
				} else {
					axialLength --;
				}
				SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
				SleepMillisecond(MillisecondDelayPerStep);
			}
			break;

		case MoveZ_AxisHeight:
			while(TargetValue != zAxisHeight)
			{
				if(zAxisHeight < TargetValue)
				{
					zAxisHeight ++;
				} else {
					zAxisHeight --;
				}
				SetOpenLoopClawPosition(rotationAngle, axialLength, zAxisHeight);
				SleepMillisecond(MillisecondDelayPerStep);
			}
			break;

		default:
			break;
	}
	return ArmControlOK;
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
	if(mode == OpenLoopMode)
	{
		//Simple logical judgment.
		if((Z_AxisHeight > Z_AxisMaximumHeight + ArmNode3_Length)
				|| (Z_AxisHeight < Z_AxisMinimumHeight - ArmNode3_Length)
				|| (AxialLength > AL_AxisMaximumLength + ArmNode3_Length + ArmNode2_Length)
				|| (AxialLength < AL_AxisMinimumLength + ArmNode3_Length)
				|| (RotationAngle > ArmRotationServoMaximumRotationAngle)
				|| (RotationAngle < 0))
		{
			return CoordinatesTooFar;
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
				return CoordinatesTooFar;
			}

			if((zAxisTargetSteps / Z_AxisStepsPerMillimeter) < Z_AxisMinimumHeight - Z_AxisMaximumHeight)
			{
				return CoordinatesTooFar;
			}

			if(armElongationAngle > MaximumElongationAngle)
			{
				return CoordinatesTooFar;
			}

			currentRotateAngle = RotationAngle;
			currentAxisLength = AxialLength;
			currentZ_AxisHeight = Z_AxisHeight;

			SetStepperSteps(&alAxisLengthStepper, alAxisTargetSteps);
			SetStepperSteps(&zAxisStepper, zAxisTargetSteps);
			SetArmNodeAngle(ArmRotation, currentRotateAngle);
			SetArmNodeAngle(ArmElongation, armElongationAngle);
			SetArmNodeAngle(ArmParallel, armParallelAngle);
		}
		return ArmControlOK;
	} else
	{
		return ControlModeError;
	}
}

/**
 * @brief: Get claw position in **Polar coordinates** in Open loop control system.
 * @param: Pointer of parameters.
 */
ArmControlResult_t GetOpenLoopClawPosition(uint16_t *RotationAngle, uint16_t *AxialLength, uint16_t *Z_AxisHeight)
{
	if(mode == OpenLoopMode)
	{
		*RotationAngle = currentRotateAngle;
		*AxialLength = currentAxisLength;
		*Z_AxisHeight = currentZ_AxisHeight;
		return ArmControlOK;
	}else{
		return ControlModeError;
	}

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
ArmControlResult_t AimAt(Target_t Target, mtime_t TimeOut)
{
	if(mode == OpenLoopMode)
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

			case FruitFocus:
				GetFruitsFocusCoordinates(&coordinates);
				if((GetCurrentTimeMillisecond() - coordinates.TimeStamp < (1000.0 * MaximumFPS_Fluctuation / TargetDetectionAverageFPS))
					&& (GetCurrentTimeMillisecond() - lastCalculateTime > (1000.0 * MaximumFPS_Fluctuation / TargetDetectionAverageFPS)))
				{
					currentX = coordinates.X;
					currentY = coordinates.Y;
					if(fabs(currentX - FruitAimCenterX) > AimToleranceErrorX)
					{
						calculateX_AxisPID(currentX, FruitAimCenterX);
						startStableTime = GetCurrentTimeMillisecond();
					}

					if(fabs(currentY - FruitAimCenterY) > AimToleranceErrorY)
					{
						calculateY_AxisPID(currentY, FruitAimCenterY);
						startStableTime = GetCurrentTimeMillisecond();
					}

					SetOpenLoopClawPosition(xAxisPID_Accumulator, yAxisPID_Accumulator, currentZ_AxisHeight);
					lastCalculateTime = GetCurrentTimeMillisecond();
				} else if(GetCurrentTimeMillisecond() - coordinates.TimeStamp > (1000.0 * MaximumFPS_Fluctuation / TargetDetectionAverageFPS)) {
					startStableTime = GetCurrentTimeMillisecond();
				}
				break;

				default:
				return AimTargetInvalid;
			}

			if(GetCurrentTimeMillisecond() - startStableTime > ClawStableTimesLimit)
			{
				xAxisPID._sumError = 0;
				yAxisPID._sumError = 0;
				return ArmControlOK;
			}

			if(GetCurrentTimeMillisecond() - startTime > TimeOut)
			{
				xAxisPID._sumError = 0;
				yAxisPID._sumError = 0;
				return AimingTimeout;
			}

			LogJustFloat(coordinates.X, TargetAimCenterX, coordinates.Y, TargetAimCenterY, coordinates.TimeStamp, startStableTime);
		}
	} else {
		return ControlModeError;
	}
}


ArmControlResult_t SetZ_AxisHeight(uint16_t Heigth)
{
	if(mode == ManualMode)
	{
		if((Heigth <= Z_AxisMaximumHeight) && (Heigth >= Z_AxisMinimumHeight))
		{
			SetStepperSteps(&zAxisStepper, (Heigth - Z_AxisZeroPoint) * Z_AxisStepsPerMillimeter);
			return ArmControlOK;
		} else {
			return CoordinatesTooFar;
		}
	} else {
		return ControlModeError;
	}
}

ArmControlResult_t SetAL_AxisLength(uint16_t Legnth)
{
	if(mode == ManualMode)
	{
		if((Legnth <= AL_AxisMaximumLength) && (Legnth >= AL_AxisMinimumLength))
		{
			SetStepperSteps(&alAxisLengthStepper, (Legnth - AL_AxisMinimumLength) * AL_AxisStepsPerMillimeter);
			return ArmControlOK;
		} else {
			return CoordinatesTooFar;
		}
	} else {
		return ControlModeError;
	}
}

uint16_t GetZ_AxisHeight(void)
{
	return 0;
}

uint16_t GetAL_AxisHeight(void)
{
	return 0;
}


#endif
