/*
 * USARTInterrupts.c
 *
 *  Created on: 2021Äê6ÔÂ12ÈÕ
 *      Author: h13
 */
#include "USARTInterrupts.h"
#include "RobotConfigs.h"

#include "SKTOF.h"
#include "WT101.h"
#include "Debug.h"
#include "ImageProcessingModule.h"

#ifdef DEBUG
/**
 * @brief: Serial Debug Interrupt Handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void SerialDebugInterruptHandler(uint8_t data)
{
	DebugHandler(data);
}
#endif

/**
 * @brief: TOF Interrupt Handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void TOF_InterruptHandler(uint8_t data)
{
	TOF_Handler(data);
}

#if(DirectionSensorModel == WT101_InSerialMode)
__attribute__((always_inline)) inline void WT101_InterruptHandler(uint8_t data)
{
	WT101_Handler(data);
}
#endif

__attribute__((always_inline)) inline void ImageProcessingModuleInterruptHandler(uint8_t data)
{
	ImageProcessingModuleHandler(data);
}

