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
	const float MinimumRotationAngle;
	const float RotationProportion;
	const float RotationOffset;
	float CurrentAngle;
} Servo_t;

static Servo_t armRotationServo    = { .PWM = (PWM_t)ArmRotationServo, .MaximumRotationAngle = ArmRotationServoMaximumRotationAngle, .MinimumRotationAngle = ArmRotationServoMinimumRotationAngle, .RotationProportion = ArmRotationServoProportion, .RotationOffset = ArmRotationServoOffset, .CurrentAngle = 0.0 };
static Servo_t armElongationServo0 = { .PWM = (PWM_t)ArmElongationServo0, .MaximumRotationAngle = ArmElongationServo0_MaximumRotationAngle, .MinimumRotationAngle = ArmElongationServo0_MinimumRotationAngle, .RotationProportion = ArmElongationServo0_Proportion, .RotationOffset = ArmElongationServo0_Offset, .CurrentAngle = 0.0 };
static Servo_t armElongationServo1 = { .PWM = (PWM_t)ArmElongationServo1, .MaximumRotationAngle = ArmElongationServo1_MaximumRotationAngle, .MinimumRotationAngle = ArmElongationServo1_MinimumRotationAngle, .RotationProportion = ArmElongationServo1_Proportion, .RotationOffset = ArmElongationServo1_Offset, .CurrentAngle = 0.0 };
static Servo_t clawRotationServo   = { .PWM = (PWM_t)ClawRotationServo, .MaximumRotationAngle = 180, .MinimumRotationAngle = 0, .RotationProportion = 1.0, .RotationOffset = 0.0, .CurrentAngle = 0.0 };
static Servo_t clawGrabServo       = { .PWM = (PWM_t)ClawGrabServo, .MaximumRotationAngle = 180, .MinimumRotationAngle = 0.0, .RotationOffset = 0.0, .CurrentAngle = 0.0 };

#if(ArmType == MechanicalArm)
static float node3_Angle = 0;
static Servo_t armElongationServo2 = { .PWM = (PWM_t)ArmElongationServo2, .MaximumRotationAngle = ArmElongationServo2_MaximumRotationAngle, .MinimumRotationAngle = ArmElongationServo2_MinimumRotationAngle, .RotationProportion = ArmElongationServo2_Proportion, .RotationOffset = ArmElongationServo2_Offset };
#endif

/**
 * @brief: Control servos movement to the calibration point to facilitate mechanical calibration.
 */
void CalibrationAllServo(void)
{
#if(ArmType == MechanicalArm)
	SetArmNodeAngle(ArmRotation, 0);
	SetArmNodeAngle(ArmElongationNode0, 0);
	SetArmNodeAngle(ArmElongationNode1, 0);
	SetArmNodeAngle(ArmElongationNode2, 0);
	SetArmNodeAngle(ArmClawRotation, 0);
	SetArmNodeAngle(ArmClawGrab, 0);
#elif(ArmType == LiftingPlatform)
	SetArmNodeAngle(ArmRotation, 0);
	SetArmNodeAngle(ArmElongationNode0, 0);
	SetArmNodeAngle(ArmElongationNode1, 0);
	SetArmNodeAngle(ArmClawRotation, 0);
	SetArmNodeAngle(ArmClawGrab, 0);
#endif
}

__attribute__((always_inline)) inline static uint16_t calculateServoHighLevelTime(float Frequence, uint16_t ReloadValue, float Angle)
{
	return Angle * 1000 / Frequence
}

/**
 * @brief: Set the rotation angle of the Arm node.
 * @param:
 * 		ArmNode_t Node: The arm node to be set.
 * 		float Angle:    Angle to rotate.
 */
void SetArmNodeAngle(ArmNode_t *Node, float Angle)
{
	float finalAngle = 0;
	switch(Node)
	{
	case ArmRotation:
		finalAngle = Angle + armRotationServo.RotationOffset;
		if(finalAngle > armRotationServo.MaximumRotationAngle)
		{
			finalAngle = maximumAngle;
			Angle = armRotationServo.MaximumRotationAngle - armRotationServo.RotationOffset;
		} else if(Angle < armRotationServo.MinimumRotationAngle)
		{
			finalAngle = minimumAngle;
			Angle = armRotationServo.MinimumRotationAngle - armRotationServo.RotationOffset;
		}

		setServoAngle(&armRotationServo.PWM, finalAngle);
		armRotationServo.CurrentAngle = Angle;
		break;

	case ArmElongationNode0:
		finalAngle = Angle + armElongationServo0.RotationOffset;
		if(finalAngle > armElongationServo0.MaximumRotationAngle)
		{
			finalAngle = maximumAngle;
			Angle = armElongationServo0.MaximumRotationAngle - armElongationServo0.RotationOffset;
		} else if(Angle < armElongationServo0.MinimumRotationAngle)
		{
			finalAngle = minimumAngle;
			Angle = armElongationServo0.MinimumRotationAngle - armElongationServo0.RotationOffset;
		}

		setServoAngle(&armElongationServo0.PWM, finalAngle);
		armElongationServo0.CurrentAngle = Angle;
		break;

	case ArmElongationNode1:
		finalAngle = Angle + armElongationServo1.RotationOffset;
		if(finalAngle > armElongationServo1.MaximumRotationAngle)
		{
			finalAngle = maximumAngle;
			Angle = armElongationServo1.MaximumRotationAngle - armElongationServo1.RotationOffset;
		} else if(Angle < armElongationServo1.MinimumRotationAngle)
		{
			finalAngle = minimumAngle;
			Angle = armElongationServo1.MinimumRotationAngle - armElongationServo1.RotationOffset;
		}

		setServoAngle(&armElongationServo1.PWM, finalAngle);
		armElongationServo1.CurrentAngle = Angle;
		break;

#if(ArmType == MechanicalArm)
	case ArmElongationNode3:
		finalAngle = Angle + armElongationServo3.RotationOffset;
		if(finalAngle > armElongationServo3.MaximumRotationAngle)
		{
			finalAngle = maximumAngle;
			Angle = armElongationServo3.MaximumRotationAngle - armElongationServo3.RotationOffset;
		} else if(Angle < armElongationServo3.MinimumRotationAngle)
		{
			finalAngle = minimumAngle;
			Angle = armElongationServo3.MinimumRotationAngle - armElongationServo3.RotationOffset;
		}

		setServoAngle(&armElongationServo3.PWM, finalAngle);
		armElongationServo3.CurrentAngle = Angle;
		break;
#endif

	case ArmClawRotation:
		finalAngle = Angle + clawRotationServo.RotationOffset;
		if(finalAngle > armElongationServo0.MaximumRotationAngle)
		{
			finalAngle = maximumAngle;
			Angle = clawRotationServo.MaximumRotationAngle - clawRotationServo.RotationOffset;
		} else if(Angle < clawRotationServo.MinimumRotationAngle)
		{
			finalAngle = minimumAngle;
			Angle = clawRotationServo.MinimumRotationAngle - clawRotationServo.RotationOffset;
		}

		setServoAngle(&clawRotationServo.PWM, finalAngle);
		clawRotationServo.CurrentAngle = Angle;
		break;

	case ArmClawGrab:
		finalAngle = Angle + clawGrabServo.RotationOffset;
		if(finalAngle > clawGrabServo.MaximumRotationAngle)
		{
			finalAngle = maximumAngle;
			Angle = clawGrabServo.MaximumRotationAngle - clawGrabServo.RotationOffset;
		} else if(Angle < clawGrabServo.MinimumRotationAngle)
		{
			finalAngle = minimumAngle;
			Angle = clawGrabServo.MinimumRotationAngle - clawGrabServo.RotationOffset;
		}

		setServoAngle(&clawGrabServo.PWM, finalAngle);
		clawGrabServo.CurrentAngle = Angle;
		break;

	default:
		break;
	}
}

/**
 * @brief:  Get the rotation angle of the Arm node.
 * @param:
 * 		ArmNode_t Node: The arm node to get.
 * @return: Current angle.
 */
float GetArmNodeAngle(ArmNode_t *Node)
{
	switch(Node)
	{
	case ArmRotation:
		return armRotationServo.CurrentAngle;
		break;

	case ArmElongationNode0:
		return armElongationServo0.CurrentAngle;
		break;

	case ArmElongationNode1:
		return armElongationServo1.CurrentAngle;
		break;

	case ArmClawRotation:
		return clawRotationServo.CurrentAngle;
		break;

	case ArmClawGrab:
		return clawGrabServo.CurrentAngle;
		break;

	default:
		return 0;
		break;
	}
}

