/*
 * WT101.c
 *
 *  Created on: 2021Äê6ÔÂ22ÈÕ
 *      Author: h13
 */
#include "WT101.h"
#include "ports.h"

#define Clockwise     0
#define AntiClockwise 1
#define PositiveDirection Clockwise

struct WT101BufferStatusStructure
{
	uint8_t header      : 1;
	uint8_t regStatus   : 1;
	uint8_t regType     : 1;
	uint8_t bufferIndex : 5;
};

static struct WT101BufferStatusStructure status = {0, 0, 0, 0};

#define Reset 0
#define Set   1

#define AngularSpeed 0
#define EulerAngle   1

static uint8_t buffer[4] = { 0x00 };
static int16_t rawYawData = 0;
static int16_t rawYawVelocityData = 0;
static int16_t rawTemperatureData = 0;

/**
 * @brief: WT101 Serial Handler.
 * @param: data.
 */
void WT101_Handler(uint8_t data)
{
	//Check Header (0x55).
	if((data == 0x55) && (status.header == Reset))
	{
		status.header = Set;
		return;
	}

	//Check register (0x52 -> Angular Speed).
	if((data == 0x52) && (status.regStatus == Reset))
	{
		if(status.header == Set)
		{
			status.regStatus = Set;
			status.regType = AngularSpeed;
		} else {
			status = (struct WT101BufferStatusStructure){0, 0, 0, 0};
		}
		return;
	}

	//Check register (0x53 -> Eular Angular).
	if((data == 0x53) && (status.regStatus == Reset))
	{
		if(status.header == Set)
		{
			status.regStatus = Set;
			status.regType = EulerAngle;
		} else {
			//Reset status.
			status = (struct WT101BufferStatusStructure){0, 0, 0, 0};
		}
		return;
	}

	//Receive data when the header and register states are Set, otherwise reset status.
	if((status.header == Set) && (status.regStatus == Set))
	{
		if((status.bufferIndex > 3) && (status.bufferIndex < 8))
		{
			buffer[status.bufferIndex - 4] = data;
		} else if(status.bufferIndex == 8)
		{
			uint8_t sum = 0;
			for(uint8_t index = 0; index < 4; index ++)
			{
				sum += buffer[index];
			}

			sum = sum + 0x55 + 0x52 + status.regType;

			if(sum == data)
			{
				if(status.regType == AngularSpeed)
				{
					rawYawVelocityData = buffer[0] | buffer[1] << 8;
				} else {
					rawYawData = buffer[0] | buffer[1] << 8;
					rawTemperatureData = buffer[2] | buffer[3] << 8;
				}
			}

			//Reset status.
			status = (struct WT101BufferStatusStructure){0, 0, 0, 0};
			return;
		}

		status.bufferIndex ++;
	} else {
		//Reset status.
		status = (struct WT101BufferStatusStructure){0, 0, 0, 0};
	}
}

/**
 * @brief: Reset yaw value to zero.
 */
void ResetYaw(void)
{
	WT101SerialSend(0xff);
	WT101SerialSend(0xaa);
	WT101SerialSend(0x76);
	WT101SerialSend(0x00);
	WT101SerialSend(0x00);
}

/**
 * @brief: Set WT101 Return Rate.
 * @note:
 * 		**This will not take effect until WT101 is repowered**
 */
void SetWT101_ReturnRateTo(WT101_ReturnRate_t Rate)
{
	WT101SerialSend(0xff);
	WT101SerialSend(0xaa);
	WT101SerialSend(0x03);
	WT101SerialSend(Rate);
	WT101SerialSend(0x00);
}

/**
 * @brief:  Get Yaw value.
 * @return: Yaw value in angle.
 */
__attribute__((always_inline)) inline float GetYawValue(void)
{
#if(PositiveDirection == Clockwise)
	return - rawYawData * 180.0 / 32768;
#else
	return rawYawData * 180.0 / 32768;
#endif
}

/**
 * @brief:  Get Yaw Velocity value.
 * @return: Yaw Velocity value in angle per second.
 */
__attribute__((always_inline)) inline float GetYawVelocity(void)
{
#if(PositiveDirection == Clockwise)
	return - rawYawVelocityData * 2000.0 / 32768;
#else
	return rawYawVelocityData * 2000.0 / 32768;
#endif
}

/**
 * @brief:  Get Temperature value.
 * @return: Temperature value in centigrade.
 */
__attribute__((always_inline)) inline float GetTemperature(void)
{
	return rawTemperatureData / 100.0;
}
