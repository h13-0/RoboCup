/*
 * SPI.h
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_SPI_SPI_H_
#define PLATFORM_SPI_SPI_H_

#include <stdint.h>

/**
 * @brief: Initialize the SPI interface.
 * @note:  If multiple devices are mounted under the SPI interface, be aware of SPI initialization sequence and repeated initialization issues.
 */
void SPIInit(void);

/**
 * @brief: SPI write 8-bit data.
 * @param: data.
 */
void SPIWriteByte(uint8_t data);

/**
 * @group: Set and Reset Pins of LCD.
 */
void LCDSetRST(void);

void LCDResetRST(void);

void LCDSetCS(void);

void LCDResetCS(void);

void LCDSetDC(void);

void LCDResetDC(void);

#endif /* PLATFORM_SPI_SPI_H_ */
