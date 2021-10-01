/*
 * USARTInterrupts.h
 *
 *  Created on: 2021Äê6ÔÂ12ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_INTERRUPTS_USARTINTERRUPTS_USARTINTERRUPTS_H_
#define PLATFORM_INTERRUPTS_USARTINTERRUPTS_USARTINTERRUPTS_H_

#include "RobotConfigs.h"
#include <stdint.h>

/**
 * @brief: Serial Debug Interrupt Handler.
 * @param: data.
 */
void SerialDebugInterruptHandler(uint8_t data);

/**
 * @brief: TOF Interrupt Handler.
 * @param: data.
 */
void TOF_InterruptHandler(uint8_t data);

#if(DirectionSensorModel == WT101_InSerialMode)
void WT101_InterruptHandler(uint8_t data);
#endif

void ImageProcessingModuleInterruptHandler(uint8_t data);

#endif /* PLATFORM_INTERRUPTS_USARTINTERRUPTS_USARTINTERRUPTS_H_ */
