/*
 * PWM.h
 *
 *  Created on: 2021Äê8ÔÂ27ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_PWM_PWM_H_
#define PLATFORM_PWM_PWM_H_

#include "RobotConfigs.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_tim.h"
#endif

typedef struct
{
	uint32_t      Frequency;
	/********Platform Code********/
	TIM_Ports_t   *TIMx;
	TIM_Channel_t Channel;
	uint32_t      ReloadValue;
} PWM_t;

/**
 * @brief: Initialize PWM peripherals.
 * @param: PWM_t *PWM: PWM typedef.
 */
void PWM_Init(PWM_t *PWM);

/**
 * @brief: Set PWM high level time.
 * @param:
 * 		PWM_t *PWM:                       The PWM peripheral you want set.
 * 		float HighLevelTimeInMicrosecond: High level time in microsecond.
 */
void SetPWM_HighLevelTime(PWM_t *PWM, float HighLevelTimeInMicrosecond);

/**
 * @brief: Set PWM duty ratio.
 * @param:
 *		PWM_t *PWM:      The PWM peripheral you want set.
 *		float DutyRatio: The duty ratio of PWM. Range: [0, 1]
 */
void SetPWM_DutyRatio(PWM_t *PWM, float DutyRatio);

#endif /* PLATFORM_PWM_PWM_H_ */
