/*
 * MotorPorts.h
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_MOTORPORTS_MOTORPORTS_H_
#define PLATFORM_MOTORPORTS_MOTORPORTS_H_

#include <stdint.h>

/**
 * @brief: Init Motor of the robot.
 */
void MotorInit(void);

/**
 * @brief: Set Value Of Channels
 * @param:
 * 		value: Value of Channels(Absolute value is less than maxPWMValue)
 */

void setChannel1Value(uint16_t value);

void setChannel2Value(uint16_t value);

void setChannel3Value(uint16_t value);

void setChannel4Value(uint16_t value);

#endif /* PLATFORM_MOTORPORTS_MOTORPORTS_H_ */
