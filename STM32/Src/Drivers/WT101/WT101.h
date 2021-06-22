/*
 * WT101.h
 *
 *  Created on: 2021Äê6ÔÂ22ÈÕ
 *      Author: h13
 */

#ifndef DRIVERS_WT101_WT101_H_
#define DRIVERS_WT101_WT101_H_

#include <stdint.h>

/**
 * @brief: WT101 Serial Handler.
 * @param: data.
 */
void WT101Handler(uint8_t data);

/**
 * @brief:  Get Yaw value.
 * @return: Yaw value in angle.
 */
float GetYawValue(void);

/**
 * @brief:  Get Yaw Velocity value.
 * @return: Yaw Velocity value in angle per second.
 */
float GetYawVelocity(void);

/**
 * @brief:  Get Temperature value.
 * @return: Temperature value in centigrade.
 */
float GetTemperature(void);

#endif /* DRIVERS_WT101_WT101_H_ */
