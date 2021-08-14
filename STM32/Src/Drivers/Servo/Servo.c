/*
 * Servo.c
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#include "Servo.h"
#include "ServoPorts.h"
#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_tim.h"
#endif

/**
 * @brief: Control servos movement to the calibration point to facilitate mechanical calibration.
 */
void CalibrationAllServo(void)
{
	SetServo0_Time(50);
	SetServo1_Time(50);
	SetServo2_Time(250);
	SetServo3_Time(150);
	SetServo4_Time(50);
	SetServo5_Time(250);
}

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [0, 180]
 */
void ArmNode0_Rotate(float Angle)
{
	Angle += Node0_ServoOffset;

	if(Angle > Node0_ServoMaximumRotationAngle)
	{
		Angle = 180;
	} else if(Angle < Node0_ServoMinimumRotationAngle)
	{
		Angle = 0;
	}

	SetServo0_Time(50 + (Angle / 180.0) * 200.0 * Node0_ServoProportion);
}

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [0, 180]
 */
void ArmNode1_Rotate(float Angle)
{
	Angle += Node1_ServoOffset;

	if(Angle > Node1_ServoMaximumRotationAngle)
	{
		Angle = 180;
	} else if(Angle < Node1_ServoMinimumRotationAngle)
	{
		Angle = 0;
	}

	SetServo1_Time(50 + (Angle / 180.0) * 200.0 * Node1_ServoProportion);
}

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [0, 180]
 */
void ArmNode2_Rotate(float Angle)
{
	Angle += Node2_ServoOffset;

	if(Angle > Node2_ServoMaximumRotationAngle)
	{
		Angle = 180;
	} else if(Angle < Node2_ServoMinimumRotationAngle)
	{
		Angle = 0;
	}

	SetServo2_Time(250 - (Angle / 180.0) * 200.0 * Node2_ServoProportion);
}

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [-90, 90]
 */
void ArmNode3_Rotate(float Angle)
{
	Angle += Node3_ServoOffset;

	if(Angle > 90)
	{
		Angle = 90;
	} else if(Angle < -90)
	{
		Angle = -90;
	}

	SetServo3_Time(150 - (Angle / 180.0) * 200.0 * Node3_ServoProportion);
}

/**
 * @brief: Rotate the claw.
 * @param: Angle, Rotation range: [0, 180]
 */
void ClawRotate(float Angle)
{
	if(Angle > 180)
	{
		Angle = 180;
	} else if(Angle < 0)
	{
		Angle = 0;
	}

	SetServo4_Time(50 + (Angle / 180.0) * 200.0);
}

/**
 * @brief: Open or close the claw.
 * @param: Angle, Rotation range: [0, 180]
 * @note:
 * 		When grasping an apple, it is recommended that the maximum rotation angle be less than `MaximumRotationAngleOfGraspingApple`.
 */
void ClawGrab(float Angle)
{
	if(Angle > 180)
	{
		Angle = 180;
	} else if(Angle < 0)
	{
		Angle = 0;
	}
	SetServo5_Time(50 + (Angle / 180.0) * 200.0);
}
