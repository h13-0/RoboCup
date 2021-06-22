/*
 * WT101.c
 *
 *  Created on: 2021��6��22��
 *      Author: h13
 */


#include "WT101.h"

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
void WT101Handler(uint8_t data)
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
 * @brief:  Get Yaw value.
 * @return: Yaw value in angle.
 */
__attribute__((always_inline)) inline float GetYawValue(void)
{
	return rawYawData * 180.0 / 32768;
}

/**
 * @brief:  Get Yaw Velocity value.
 * @return: Yaw Velocity value in angle per second.
 */
__attribute__((always_inline)) inline float GetYawVelocity(void)
{
	return rawYawVelocityData * 2000.0 / 32768;
}

/**
 * @brief:  Get Temperature value.
 * @return: Temperature value in centigrade.
 */
__attribute__((always_inline)) inline float GetTemperature(void)
{
	return rawTemperatureData / 100.0;
}

