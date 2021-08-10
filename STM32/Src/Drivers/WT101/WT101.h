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
void WT101_Handler(uint8_t data);

/**
 * @brief: Reset yaw value to zero.
 */
void ResetYaw(void);

typedef enum
{
	Rate0_1Hz = 0x01,
	Rate0_5Hz = 0x02,
	Rate1Hz   = 0x03,
	Rate2Hz   = 0x04,
	Rate5Hz   = 0x05,
	Rate10Hz  = 0x06,
	Rate20Hz  = 0x07,
	Rate50Hz  = 0x08,
	Rate100Hz = 0x09,
	Rate125Hz = 0x0a,
	Rate250Hz = 0x0b,
	Rate500Hz = 0x0c,
} WT101_ReturnRate_t;

/**
 * @brief: Set WT101 Return Rate.
 * @note:
 * 		**This will not take effect until WT101 is repowered**
 */
void SetWT101_ReturnRateTo(WT101_ReturnRate_t Rate);

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
