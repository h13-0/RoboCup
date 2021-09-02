/*
 * PWM.c
 *
 *  Created on: 2021Äê8ÔÂ27ÈÕ
 *      Author: h13
 */
#include "PWM.h"

/**
 * @brief: Initialize PWM peripherals.
 * @param: PWM_t *PWM: PWM typedef.
 */
void PWM_Init(PWM_t *PWM)
{
	LL_TIM_CC_EnableChannel(PWM -> TIMx, PWM -> Channel);

	if(IS_TIM_REPETITION_COUNTER_INSTANCE(PWM -> TIMx))
	{
		LL_TIM_EnableAllOutputs(PWM -> TIMx);
	}

	LL_TIM_EnableCounter(PWM -> TIMx);
}

/**
 * @brief: Set PWM high level time.
 * @param:
 * 		PWM_t *PWM:                       The PWM peripheral you want set.
 * 		float HighLevelTimeInMicrosecond: High level time in microsecond.
 */
void SetPWM_HighLevelTime(PWM_t *PWM, float HighLevelTimeInMicrosecond)
{
	//DutyRatio = HighLevelTimeInMicrosecond * PWM -> Frequency / 1000000
	float dutyRatio = HighLevelTimeInMicrosecond * PWM -> Frequency / 1000000;
	uint32_t compareValue = PWM -> ReloadValue * dutyRatio;
	switch(PWM -> Channel)
	{
	case LL_TIM_CHANNEL_CH1:
		LL_TIM_OC_SetCompareCH1(PWM -> TIMx, compareValue);
		break;

	case LL_TIM_CHANNEL_CH2:
		LL_TIM_OC_SetCompareCH2(PWM -> TIMx, compareValue);
		break;

	case LL_TIM_CHANNEL_CH3:
		LL_TIM_OC_SetCompareCH3(PWM -> TIMx, compareValue);
		break;

	case LL_TIM_CHANNEL_CH4:
		LL_TIM_OC_SetCompareCH4(PWM -> TIMx, compareValue);
		break;

	default:
		break;
	}
}

/**
 * @brief: Set PWM duty ratio.
 * @param:
 *		PWM_t *PWM:      The PWM peripheral you want set.
 *		float DutyRatio: The duty ratio of PWM. Range: [0, 1]
 */
void SetPWM_DutyRatio(PWM_t *PWM, float DutyRatio)
{
	uint32_t compareValue = PWM -> ReloadValue * DutyRatio;
	switch(PWM -> Channel)
	{
	case LL_TIM_CHANNEL_CH1:
		LL_TIM_OC_SetCompareCH1(PWM -> TIMx, compareValue);
		break;

	case LL_TIM_CHANNEL_CH2:
		LL_TIM_OC_SetCompareCH2(PWM -> TIMx, compareValue);
		break;

	case LL_TIM_CHANNEL_CH3:
		LL_TIM_OC_SetCompareCH3(PWM -> TIMx, compareValue);
		break;

	case LL_TIM_CHANNEL_CH4:
		LL_TIM_OC_SetCompareCH4(PWM -> TIMx, compareValue);
		break;

	default:
		break;
	}
}
