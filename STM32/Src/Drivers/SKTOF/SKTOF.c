/*
 * SKTOF.c
 *
 *  Created on: 2021Äê6ÔÂ11ÈÕ
 *      Author: h13
 */
#include "SKTOF.h"

#include <stdint.h>
#include <string.h>

#include "BitOperation.h"

static uint16_t distance;

#define BUFFERLENGTH  17

/**
 * @brief: status of Buffer
 * @bits:
 * 		bit0:        0x0D status.
 * 		bit1--bit15: length status.
 */
volatile static uint8_t bufferStatus;
#define setH0DStatus()    	Set8Bit0(bufferStatus)
#define getH0DStatus()      Get8Bit0(bufferStatus)
#define getBufferLength()   (bufferStatus & 0x7f)

static char buffer[BUFFERLENGTH] = { '\0' };

static void getDistanceHandler(char* data);

/**
 * @brief: TOF Serial Handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void TOFHandler(uint8_t data)
{
	if (getH0DStatus()) {
		if (data != 0x0a) {
			bufferStatus = 0;
		} else {
			if(getBufferLength() == 17)
			{
				/**
				 * @group: Line Message Handlers
				 * @param:
				 * 		char *buffer:               String
				 * 		uint16_t getBufferLength(): length
				 */
				//Line Message Handlers.
				getDistanceHandler(buffer);
			}
			//Reset Buffer and BufferStatus
			bufferStatus = 0;
			memset(buffer, '\0', BUFFERLENGTH);
		}
	} else {
		if (data == 0x0d) {
			setH0DStatus();
		} else {
			if(getBufferLength() > (BUFFERLENGTH - 1))
			{
				//Error
				//Reset Buffer and BufferStatus
				memset(buffer, '\0', BUFFERLENGTH);
				buffer[0] = data;
				bufferStatus = 1;
			}else{
				buffer[getBufferLength()] = data;
				bufferStatus ++;
			}
		}
	}
}

static __attribute__((always_inline)) inline void getDistanceHandler(char* data)
{
	//Check leading character.
	if(*data == '~')
	{
		data += 3;
		//Check register.
		if(!memcmp(data, "030100", 6))
		{
			data += 6;
			//Write data to variable
			uint16_t tempDistance = 0;

			//'Data_Hh' 'Data_Hl' 'Data_Lh' 'Data_Hl'
			//To:
			//'Data_Lh' 'Data_Hl' 'Data_Hh' 'Data_Hl'
			//'Data_L' 'Data_H'
			//tempDistance += (((*data) - '0') & 0x0f) << 12;
			tempDistance = (((*data) - '0') & 0x0f) << 12;
			data ++;

			//tempDistance += (((*data) - '0') & 0x0f) << 8;
			tempDistance |= (((*data) - '0') & 0x0f) << 8;
			data ++;

			//tempDistance += (((*data) - '0') & 0x0f) << 4;
			tempDistance |= (((*data) - '0') & 0x0f) << 4;
			data ++;

			//tempDistance |= (((*data) - '0') & 0x0f);
			tempDistance |= (((*data) - '0') & 0x0f) << 0;
			distance = tempDistance;
		}
	}
}

/**
 * @brief:  Get TOF Distance value.
 * @return: Distance value in millimeters.
 */
__attribute__((always_inline)) inline uint16_t GetTOFDistance(void)
{
	return distance;
}
