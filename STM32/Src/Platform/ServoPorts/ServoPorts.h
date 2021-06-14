/*
 * ServoPorts.h
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_SERVOPORTS_SERVOPORTS_H_
#define PLATFORM_SERVOPORTS_SERVOPORTS_H_

#include <stdint.h>

/**
 * @brief: Set the high level time of Servos
 * @param: High Level Time of Servo, the unit is 0.01 ms
 * 		ParamRange: 50~250
 */
void ServoInit(void);

/**
 * @group: API for setting high level times for all servos.
 */
void SetServo0Time(uint16_t HighLevelTime);

void SetServo1Time(uint16_t HighLevelTime);

void SetServo2Time(uint16_t HighLevelTime);

void SetServo3Time(uint16_t HighLevelTime);

void SetServo4Time(uint16_t HighLevelTime);

void SetServo5Time(uint16_t HighLevelTime);

#endif /* PLATFORM_SERVOPORTS_SERVOPORTS_H_ */
