/*
 * Encoder.c
 *
 *  Created on: 2021Äê8ÔÂ27ÈÕ
 *      Author: h13
 */
#include "Encoder.h"
#include "RobotConfigs.h"
#include "EncoderPorts.h"

static DoublePhaseEncoder_t leftEncoder = LeftEncoder;
static DoublePhaseEncoder_t rightEncoder = RightEncoder;

void EncoderInit(void)
{

	EncoderPortsInit(&leftEncoder);
	EncoderPortsInit(&rightEncoder);
}

int32_t GetLeftEncoderSpeed(void)
{
	return GetEncoderSpeed(&leftEncoder);
}

int32_t GetRightEncoderSpeed(void)
{
	return - GetEncoderSpeed(&rightEncoder);
}
