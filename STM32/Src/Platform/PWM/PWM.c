/*
 * PWM.c
 *
 *  Created on: 2021Äê8ÔÂ27ÈÕ
 *      Author: h13
 */
#include "PWM.h"

void PWM_Init(PWM_t *PWM, float Frequence, uint16_t ReloadValue)
{
	LL_TIM_CC_EnableChannel(PWM -> TIMx, PWM -> Channel);

	if(IS_TIM_REPETITION_COUNTER_INSTANCE(PWM -> TIMx))
	{
		LL_TIM_EnableAllOutputs(PWM -> TIMx);
	}

	LL_TIM_EnableCounter(PWM -> TIMx);
}

void SetPWM_HighLevelCompareValue(PWM_t *PWM, uint32_t CompareValue)
{
	switch(PWM -> Channel)
	{
	case LL_TIM_CHANNEL_CH1:
		LL_TIM_OC_SetCompareCH1(PWM -> TIMx, CompareValue);
		break;
	case LL_TIM_CHANNEL_CH2:
		LL_TIM_OC_SetCompareCH2(PWM -> TIMx, CompareValue);
		break;
	case LL_TIM_CHANNEL_CH3:
		LL_TIM_OC_SetCompareCH3(PWM -> TIMx, CompareValue);
		break;
	case LL_TIM_CHANNEL_CH4:
		LL_TIM_OC_SetCompareCH4(PWM -> TIMx, CompareValue);
		break;
	default:
		break;
	}
}
