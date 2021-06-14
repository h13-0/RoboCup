/*
 * Debug.h
 *
 *  Created on: 2021Äê6ÔÂ10ÈÕ
 *      Author: h13
 */

#ifndef APP_DEBUG_DEBUG_H_
#define APP_DEBUG_DEBUG_H_

#ifdef DEBUG

#include <stdint.h>

/**
 * @brief: Debug handler.
 * @param: Debug Port raw data.
 */
void DebugHandler(uint8_t data);

#endif

#endif /* APP_DEBUG_DEBUG_H_ */
