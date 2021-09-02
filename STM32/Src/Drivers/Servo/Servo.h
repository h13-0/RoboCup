/*
 * Servo.h
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_SERVO_SERVO_H_
#define PLATFORM_SERVO_SERVO_H_

#include "RobotConfigs.h"
#include "Ports.h"

void ServoInit(void);

/**
 * @brief: Control servos movement to the calibration point to facilitate mechanical calibration.
 */
void CalibrationAllServo(void);
#if(ArmType == MechanicalArm)
typedef enum
{
	ArmRotation,
	ArmElongationNode0,
	ArmElongationNode1,
	ArmParallel,
	ClawRotation,
	ClawGrab,
} ArmNode_t;
#elif(ArmType == LiftingPlatform)
typedef enum
{
	ArmRotation,
	ArmElongation,
	ArmParallel,
	ClawRotation,
	ClawGrab,
} ArmNode_t;
#endif

/**
 * @brief: Set the rotation angle of the Arm node.
 * @param:
 * 		ArmNode_t Node: The arm node to be set.
 * 		float Angle:    Angle to rotate.
 */
void SetArmNodeAngle(ArmNode_t Node, float Angle);

/**
 * @brief: Smooth rotation arm node.
 * @param:
 * 		ArmNode_t Node:                The arm node to be set.
 * 		float Angle:                   Angle to rotate.
 * 		mtime_t MillisecondsPerDegree: Milliseconds per degree.
 */
void SmoothRotateArmNode(ArmNode_t Node, float Angle, mtime_t MillisecondsPerDegree);

/**
 * @brief:  Get the rotation angle of the Arm node.
 * @param:
 * 		ArmNode_t Node: The arm node to get.
 * @return: Current angle.
 */
float GetArmNodeAngle(ArmNode_t Node);

#endif /* PLATFORM_SERVO_SERVO_H_ */
