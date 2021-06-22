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
 * @brief: Reset yaw value to zero.
 */
void ResetYaw(void);

/**
 * @brief: Set WT101 Return Rate To 100hz.
 * @note:
 * 		**This will not take effect until WT101 is repowered**
 */
void SetWT101ReturnRateTo100(void);

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
