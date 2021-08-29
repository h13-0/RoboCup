/*
 * Encoder.h
 *
 *  Created on: 2021Äê7ÔÂ24ÈÕ
 *      Author: h13
 */

#ifndef SRC_ENCODER_ENCODER_H_
#define SRC_ENCODER_ENCODER_H_

#if defined(STM32F103xB) || defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_tim.h"
#endif

#include <stdint.h>

typedef struct
{
	/************************ The TIM port that the encoder is connected to. ************************/
	TIM_TypeDef *TimPort;
	/********************* Because the counter can only get the last direction. *********************/
	/****** Therefore, the velocity direction judgment depends on the maximum velocity value. *******/
	uint32_t MaximumSpeed;
} DoublePhaseEncoder_t;

void EncoderPortsInit(DoublePhaseEncoder_t *Encoder);

/**
 * @brief: Get the speed (triggers) of the encoder.
 * @note:  This function relies on pulse counting, **so it MUST be called PERIODICALLY.**
 */
int32_t GetEncoderSpeed(DoublePhaseEncoder_t *Encoder);

#endif /* SRC_ENCODER_ENCODER_H_ */
