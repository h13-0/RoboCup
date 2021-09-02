/*
 * Servo.c
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */
#include "Servo.h"
#include "RobotConfigs.h"
#include "PWM.h"

typedef struct
{
	PWM_t PWM;
	const float MaximumRotationAngle;
	const float RotationProportion;
	const float RotationOffset;
	float CurrentAngle;
} Servo_t;

static Servo_t armRotationServo    = { .PWM = (PWM_t)ArmRotationServo,   .MaximumRotationAngle = ArmRotationServoMaximumRotationAngle,   .RotationProportion = ArmRotationServoProportion,   .RotationOffset = ArmRotationServoOffset,   .CurrentAngle = 0.0 };
static Servo_t armElongationServo  = { .PWM = (PWM_t)ArmElongationServo, .MaximumRotationAngle = ArmElongationServoMaximumRotationAngle, .RotationProportion = ArmElongationServoProportion, .RotationOffset = ArmElongationServoOffset, .CurrentAngle = 0.0 };
static Servo_t armParallelServo    = { .PWM = (PWM_t)ArmParallelServo,   .MaximumRotationAngle = ArmParallelServoMaximumRotationAngle,   .RotationProportion = ArmParallelServoProportion,   .RotationOffset = ArmParallelServoOffset,   .CurrentAngle = 0.0 };
static Servo_t clawRotationServo   = { .PWM = (PWM_t)ClawRotationServo,  .MaximumRotationAngle = 180,                                    .RotationProportion = 1.0,                          .RotationOffset = 0.0,                      .CurrentAngle = 0.0 };
static Servo_t clawGrabServo       = { .PWM = (PWM_t)ClawGrabServo,      .MaximumRotationAngle = 180,                                    .RotationProportion = 1.0,                          .RotationOffset = 0.0,                      .CurrentAngle = 0.0 };

#if(ArmType == MechanicalArm)
static float node3_Angle = 0;
static Servo_t armElongationServo2 = { .PWM = (PWM_t)ArmElongationServo2, .MaximumRotationAngle = ArmElongationServo2_MaximumRotationAngle, .MinimumRotationAngle = ArmElongationServo2_MinimumRotationAngle, .RotationProportion = ArmElongationServo2_Proportion, .MinimumRotationAngle = ArmElongationServo2_Offset };
#endif

void ServoInit(void)
{
#if(ArmType == MechanicalArm)

#elif(ArmType == LiftingPlatform)
	PWM_Init(&armRotationServo.PWM);
	PWM_Init(&armElongationServo.PWM);
	PWM_Init(&armParallelServo.PWM);
	PWM_Init(&clawRotationServo.PWM);
	PWM_Init(&clawGrabServo.PWM);
#endif
}


/**
 * @brief: Control servos movement to the calibration point to facilitate mechanical calibration.
 */
void CalibrationAllServo(void)
{
#if(ArmType == MechanicalArm)
	SetArmNodeAngle(ArmRotation, 0);
	SetArmNodeAngle(ArmElongationNode0, 0);
	SetArmNodeAngle(ArmElongationNode1, 0);
	SetArmNodeAngle(ArmParallel, 0);
	SetArmNodeAngle(ClawRotation, 0);
	SetArmNodeAngle(ClawGrab, 0);
#elif(ArmType == LiftingPlatform)
	SetArmNodeAngle(ArmRotation, 0);
	SetArmNodeAngle(ArmElongation, 0);
	SetArmNodeAngle(ArmParallel, 0);
	SetArmNodeAngle(ClawRotation, 0);
	SetArmNodeAngle(ClawGrab, 0);
#endif
}

/**
 * @brief: Set servo angle.
 * @param: Angle.
 */
__attribute__((always_inline)) inline static void setServoAngle(Servo_t *Servo, float Angle)
{
	//FinalAngle = ((Angle + Servo -> RotationOffset) * Servo -> RotationProportion)
	//HighLevelTime = (FinalAngle / Servo -> MaximumRotationAngle) * 2000.0 + 500.0
	float highLevelTime = (((Angle + Servo -> RotationOffset) * Servo -> RotationProportion) / Servo -> MaximumRotationAngle) * 2000.0 + 500.0;

	if(highLevelTime > 2500.0)
	{
		highLevelTime = 2500.0;
		Servo -> CurrentAngle = Servo -> MaximumRotationAngle;
	} else if(highLevelTime < 500.0)
	{
		highLevelTime = 500.0;
		Servo -> CurrentAngle = 0.0;
	} else {
		Servo -> CurrentAngle = Angle;
	}
	SetPWM_HighLevelTime(&(Servo -> PWM), highLevelTime);
}

/**
 * @brief: Set the rotation angle of the Arm node.
 * @param:
 * 		ArmNode_t Node: The arm node to be set.
 * 		float Angle:    Angle to rotate.
 */
void SetArmNodeAngle(ArmNode_t Node, float Angle)
{
	switch(Node)
	{
	case ArmRotation:
		setServoAngle(&armRotationServo, Angle);
		break;

	case ArmElongation:
		setServoAngle(&armElongationServo, Angle);
		break;

	case ArmParallel:
		setServoAngle(&armParallelServo, Angle);
		break;

#if(ArmType == MechanicalArm)
	case ArmElongationNode0:
		setServoAngle(&armElongationServo2, Angle);
		break;
#endif

	case ClawRotation:
		setServoAngle(&clawRotationServo, Angle);
		break;

	case ClawGrab:
		setServoAngle(&clawGrabServo, Angle);
		break;

	default:
		break;
	}
}

/**
 * @brief: Smooth rotation arm node.
 * @param:
 * 		ArmNode_t Node:                The arm node to be set.
 * 		float Angle:                   Angle to rotate.
 * 		mtime_t MillisecondsPerDegree: Milliseconds per degree.
 */
void SmoothRotateArmNode(ArmNode_t Node, float Angle, mtime_t MillisecondsPerDegree)
{
	Servo_t *servo = NULL;
	switch(Node)
	{
	case ArmRotation:
		servo = &armRotationServo;
		break;

	case ArmElongation:
		servo = &armElongationServo;
		break;

	case ArmParallel:
		servo = &armParallelServo;
		break;

	case ClawRotation:
		servo = &clawRotationServo;
		break;

	case ClawGrab:
		servo = &clawGrabServo;

	default:
		return;
	}

	while(Angle != servo -> CurrentAngle)
	{
		if(Angle > servo -> CurrentAngle)
		{
			servo -> CurrentAngle += 1.0;
		} else if(Angle < servo -> CurrentAngle)
		{
			servo -> CurrentAngle -= 1.0;
		}

		setServoAngle(servo, servo -> CurrentAngle --);

		SleepMillisecond(MillisecondsPerDegree);
	}
}

/**
 * @brief:  Get the rotation angle of the Arm node.
 * @param:
 * 		ArmNode_t Node: The arm node to get.
 * @return: Current angle.
 */
float GetArmNodeAngle(ArmNode_t Node)
{
	switch(Node)
	{
	case ArmRotation:
		return armRotationServo.CurrentAngle;
		break;

	case ArmElongation:
		return armElongationServo.CurrentAngle;
		break;

	case ArmParallel:
		return armParallelServo.CurrentAngle;
		break;

	case ClawRotation:
		return clawRotationServo.CurrentAngle;
		break;

	case ClawGrab:
		return clawGrabServo.CurrentAngle;
		break;

	default:
		return 0;
		break;
	}
}
