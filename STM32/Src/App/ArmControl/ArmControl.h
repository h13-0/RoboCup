/*
 * ArmControl.h
 *
 *  Created on: 2021Äê8ÔÂ7ÈÕ
 *      Author: h13
 */

#ifndef APP_ARMCONTROL_ARMCONTROL_H_
#define APP_ARMCONTROL_ARMCONTROL_H_

typedef enum
{
	ArmControlOK = 0,
	TooFar,
	TargetValueIsInvalid,
} ArmControlResult_t;

/**
 * @brief: Set claw position in **Polar coordinates**
 * @param:
 * 		RotationAngle: The Angle of rotation relative to the zero position. Range: [0, 180]
 * 		AxialLength:   The length of the axis relative to zero in millimeters.
 * 		Z_AxisHeight:  The height above the ground in millimeters.
 */
ArmControlResult_t SetClawPosition(float RotationAngle, float AxialLength, float Z_AxisHeight);

#endif /* APP_ARMCONTROL_ARMCONTROL_H_ */
