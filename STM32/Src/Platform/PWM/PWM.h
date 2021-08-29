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
	TIM_Ports_t   *TIMx;
	TIM_Channel_t Channel;
} PWM_t;

void PWM_Init(PWM_t *PWM, float Frequence, uint16_t ReloadValue);

void SetPWM_HighLevelCompareValue(PWM_t *PWM, uint32_t CompareValue);

#endif /* PLATFORM_PWM_PWM_H_ */
