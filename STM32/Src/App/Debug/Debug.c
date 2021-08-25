/*
 * Debug.c
 *
 *  Created on: 2021Äê6ÔÂ10ÈÕ
 *      Author: h13
 */

#ifdef DEBUG
#include "Debug.h"
#include <stdint.h>
#include <string.h>

/**
 * @group: Line Message Handlers
 */
#include "ParameterAdjust.h"

#define BUFFERLENGTH  32

#if (BUFFERLENGTH > 32768)
#error "BUFFERLENGTH must less than 32768"
#endif

/**
 * @brief: status of Buffer
 * @bits:
 * 		bit0:        0x0D status.
 * 		bit1--bit15: length status.
 */
static uint16_t bufferStatus;
#define setH0DStatus()    (bufferStatus |= 0x8000)
#define getH0DStatus()    (bufferStatus & 0x8000)
#define getBufferLength()   (bufferStatus & 0x7fff)

static char buffer[BUFFERLENGTH] = { '\0' };

/**
 * @brief: Debug handler.
 * @param: Debug Port raw data.
 */
__attribute__((always_inline)) inline void DebugHandler(uint8_t data) {
	if (getH0DStatus()) {
		if (data != 0x0a) {
			bufferStatus = 0;
		} else {
			/**
			 * @group: Line Message Handlers
			 * @param:
			 * 		char *buffer:               String
			 * 		uint16_t getBufferLength(): length
			 */
			//Adjust PID Handler
			PIDAdjustHandler(buffer, getBufferLength());

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

#endif
