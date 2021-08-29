/*
 * Encoder.c
 *
 *  Created on: 2021Äê7ÔÂ24ÈÕ
 *      Author: h13
 */

#include "EncoderPorts.h"

void EncoderPortsInit(DoublePhaseEncoder_t *Encoder)
{
	TIM_TypeDef *timPort = Encoder -> TimPort;
	LL_TIM_CC_EnableChannel(timPort, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(timPort, LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(timPort);
}

int32_t GetEncoderSpeed(DoublePhaseEncoder_t *Encoder)
{
	TIM_TypeDef *timPort = Encoder -> TimPort;
	uint32_t maximumSpeed = Encoder -> MaximumSpeed;
	uint32_t autoReloadValue = LL_TIM_GetAutoReload(timPort);
	uint32_t triggerTimes = LL_TIM_GetCounter(timPort);

	LL_TIM_SetCounter(timPort, 0);

	if(triggerTimes < maximumSpeed)
	{
		return (float)triggerTimes;
	} else {
		return - (float)(autoReloadValue - triggerTimes);
	}
}
