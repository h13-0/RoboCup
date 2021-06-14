/*
 * USARTInterrupts.c
 *
 *  Created on: 2021Äê6ÔÂ12ÈÕ
 *      Author: h13
 */
#include "USARTInterrupts.h"
#include "SKTOF.h"


/**
 * @brief: Serial Debug Interrupt Handler.
 * @param: data.
 */
void SerialDebugInterruptHandler(uint8_t data)
{

}

/**
 * @brief: TOF Interrupt Handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void TOFInterruptHandler(uint8_t data)
{
	TOFHandler(data);
}
