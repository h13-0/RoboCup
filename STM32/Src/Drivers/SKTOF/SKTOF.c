/*
 * SKTOF.c
 *
 *  Created on: 2021Äê6ÔÂ11ÈÕ
 *      Author: h13
 */
#include "SKTOF.h"

#include <stdint.h>
#include "SimpleProtocolPraise.h"

static uint16_t distance;
#define BUFFERLENGTH  24

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
			if((*data) <= '9')
			{
				tempDistance = (((*data) - '0') & 0x0f) << 12;
			} else {
				tempDistance = (((*data) - 'A' + 10) & 0x0f) << 12;
			}

			data ++;

			if((*data) <= '9')
			{
				tempDistance |= (((*data) - '0') & 0x0f) << 8;
			} else {
				tempDistance |= (((*data) - 'A' + 10) & 0x0f) << 8;
			}

			data ++;

			if((*data) <= '9')
			{
				tempDistance |= (((*data) - '0') & 0x0f) << 4;
			} else {
				tempDistance |= (((*data) - 'A' + 10) & 0x0f) << 4;
			}

			data ++;

			if((*data) <= 9)
			{
				tempDistance |= (((*data) - '0') & 0x0f) << 0;
			} else {
				tempDistance |= (((*data) - 'A' + 10) & 0x0f) << 0;
			}

			distance = tempDistance;
		}
	}
}

/**
 * @brief: TOF Serial Handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void TOF_Handler(uint8_t data)
{
	static char buffer[BUFFERLENGTH] = { '\0' };
	static uint8_t length = 0;
	static char* bufferPtr = buffer;
	//GeneratePraiseWithSuffixMethod(data, "\r\n", 2, bufferPtr, BUFFERLENGTH, length, if(length == 17) { getDistanceHandler(bufferPtr); });
	GeneratePraiseWithSuffixMethod(data, "\r\n", 2, bufferPtr, BUFFERLENGTH, length, if(length == 19) { getDistanceHandler(bufferPtr); });
}

/**
 * @brief:  Get TOF Distance value.
 * @return: Distance value in millimeters.
 */
__attribute__((always_inline)) inline uint16_t GetTOF_Distance(void)
{
	return distance;
}
