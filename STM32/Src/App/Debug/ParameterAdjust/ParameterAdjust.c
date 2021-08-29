/*
 * PIDAdjust.c
 *
 *  Created on: 2021Äê6ÔÂ10ÈÕ
 *      Author: h13
 */

#ifdef DEBUG
#include <ParameterAdjust/ParameterAdjust.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "PID.h"
#include "FastMatch.h"

#include "ArmControl.h"
#include "Servo.h"

extern PositionPID_t LeftSpeedPID, RightSpeedPID;
extern PositionPID_t AnglePID;
extern PositionPID_t ForwardPID;

extern PositionPID_t X_AxisPID;
extern PositionPID_t Y_AxisPID;

/**
 * @brief: PID Adjust Handler.
 * @param:
 * 		char    *data: pointer of string.
 * 		uint8_t len:   length of string.
 * @usage:
 *		${Key}:${hex of float}\r\n
 */
__attribute__((always_inline)) inline void PIDAdjustHandler(char *data, uint8_t len)
{
	//3 Bytes keys:
	//SpeedPID
	float floatValue = 0;
	MatchKeyFloat(data, len, "SP:", 3, floatValue, LeftSpeedPID.proportion = floatValue; RightSpeedPID.proportion = floatValue; return);

	MatchKeyFloat(data, len, "SI:", 3, floatValue, LeftSpeedPID.integration = floatValue; RightSpeedPID.integration = floatValue; return);

	MatchKeyFloat(data, len, "SD:", 3, floatValue, LeftSpeedPID.differention = floatValue; RightSpeedPID.differention = floatValue; return);

	MatchKeyFloat(data, len, "TS:", 3, floatValue, LeftSpeedPID.setpoint = floatValue; RightSpeedPID.setpoint = floatValue; return);

	//AnglePID
	MatchKeyFloat(data, len, "AP:", 3, AnglePID.proportion, return);

	MatchKeyFloat(data, len, "AI:", 3, AnglePID.integration, return);

	MatchKeyFloat(data, len, "AD:", 3, AnglePID.differention, return);

	MatchKeyFloat(data, len, "TA:", 3, AnglePID.setpoint, return);

	//ForwardPID
	MatchKeyFloat(data, len, "FP:", 3, ForwardPID.proportion, return);

	MatchKeyFloat(data, len, "FI:", 3, ForwardPID.integration, return);

	MatchKeyFloat(data, len, "FD:", 3, ForwardPID.differention, return);

	MatchKeyFloat(data, len, "TF:", 3, ForwardPID.setpoint, return);

	//4 Bytes keys:
	//SpeedPID
	MatchKeyFloat(data, len, "SMI:", 4, floatValue, LeftSpeedPID.maximumAbsValueOfIntegrationOutput = floatValue; RightSpeedPID.maximumAbsValueOfIntegrationOutput = floatValue; return);

	//AnglePID
	MatchKeyFloat(data, len, "AMI:", 4, AnglePID.maximumAbsValueOfIntegrationOutput, return);

	//ForwardPID
	MatchKeyFloat(data, len, "FMI:", 4, ForwardPID.maximumAbsValueOfIntegrationOutput, return);

	//Arm control
	MatchKeyFloat(data, len, "XAP:", 4, X_AxisPID.proportion, return);
	MatchKeyFloat(data, len, "XAI:", 4, X_AxisPID.integration, return);

	MatchKeyFloat(data, len, "YAP:", 4, Y_AxisPID.proportion, return);
	MatchKeyFloat(data, len, "YAI:", 4, Y_AxisPID.integration, return);

	//Test
	static float RotationAngle, AxialLength, Z_AxisHeight;
	MatchKeyFloat(data, len, "RA:", 3, RotationAngle, SetOpenLoopClawPosition(RotationAngle, AxialLength, Z_AxisHeight); return);
	MatchKeyFloat(data, len, "AL:", 3, AxialLength, SetOpenLoopClawPosition(RotationAngle, AxialLength, Z_AxisHeight); return);
	MatchKeyFloat(data, len, "ZA:", 3, Z_AxisHeight, SetOpenLoopClawPosition(RotationAngle, AxialLength, Z_AxisHeight); return);

	float angle = 0;
	MatchKeyFloat(data, len, "N1:", 3, angle, ArmNode1_Rotate(angle); return);
	MatchKeyFloat(data, len, "N2:", 3, angle, ArmNode2_Rotate(angle); return);

#if(ArmType == MechanicalArm)
	MatchKeyFloat(data, len, "N3:", 3, angle, ArmNode3_Rotate(angle); return);
#endif

	MatchKeyFloat(data, len, "CG:", 3, angle, ClawGrab(angle); return);

	float speed = 0;
	MatchKeyFloat(data, len, "sp:", 3, speed, SetLeftMotorPWM(speed); SetRightMotorPWM(speed); return);
}

#endif
