/*
 * USARTInterrupts.c
 *
 *  Created on: 2021Äê6ÔÂ12ÈÕ
 *      Author: h13
 */
#include "USARTInterrupts.h"
#include "SKTOF.h"
#include "WT101.h"

/**
 * @brief: Serial Debug Interrupt Handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void SerialDebugInterruptHandler(uint8_t data)
{

}

/**
 * @brief: TOF Interrupt Handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void TOFInterruptHandler(uint8_t data)
{
	TOF_Handler(data);
}

__attribute__((always_inline)) inline void WT101InterruptHandler(uint8_t data)
{
	WT101Handler(data);
}
