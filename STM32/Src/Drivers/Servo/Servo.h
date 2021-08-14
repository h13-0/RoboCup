/*
 * Servo.h
 *
 *  Created on: 2021Äê6ÔÂ4ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_SERVO_SERVO_H_
#define PLATFORM_SERVO_SERVO_H_

/**
 * @brief: Control servos movement to the calibration point to facilitate mechanical calibration.
 */
void CalibrationAllServo(void);

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [0, 270]
 */
void ArmNode0_Rotate(float Angle);

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [0, 180]
 */
void ArmNode1_Rotate(float Angle);

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [0, 180]
 */
void ArmNode2_Rotate(float Angle);

/**
 * @brief: Rotate the arm node.
 * @param: Angle, Rotation range: [0, 180]
 */
void ArmNode3_Rotate(float Angle);

/**
 * @brief: Rotate the claw.
 * @param: Angle, Rotation range: [0, 180]
 */
void ClawRotate(float Angle);

/**
 * @brief: Open or close the claw.
 * @param: Angle, Rotation range: [0, 180]
 * @note:
 * 		When grasping an apple, it is recommended that the maximum rotation angle be less than `MaximumRotationAngleOfGraspingApple`.
 */
void ClawGrab(float Angle);


#endif /* PLATFORM_SERVO_SERVO_H_ */
