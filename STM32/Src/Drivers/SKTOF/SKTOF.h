/*
 * SKTOF.h
 *
 *  Created on: 2021Äê6ÔÂ11ÈÕ
 *      Author: h13
 */

#ifndef DRIVERS_SKTOF_SKTOF_H_
#define DRIVERS_SKTOF_SKTOF_H_

#include <stdint.h>

/**
 * @brief: TOF Serial Handler.
 * @param: data.
 */
void TOFHandler(uint8_t data);

/**
 * @brief:  Get TOF Distance value.
 * @return: Distance value in millimeters.
 */
uint16_t GetTOFDistance(void);

#endif /* DRIVERS_SKTOF_SKTOF_H_ */
