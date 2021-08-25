/*
 * ServoPorts.c
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */
#include "ServoPorts.h"
#include "RobotConfigs.h"

#include <stdint.h>

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_tim.h"
#endif

#if(ArmType == MechanicalArm)

/**
 * @brief: Set the high level time of Servos
 * @param: High Level Time of Servo, the unit is 0.01 ms
 * 		ParamRange: 50~250
 */
void ServoInit(void)
{
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
	  LL_TIM_EnableAllOutputs(TIM1);
	  LL_TIM_EnableCounter(TIM1);

	  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
	  LL_TIM_EnableCounter(TIM2);
}

/**
 * @group: API for setting high level times for all servos.
 */
void SetServo0_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH1(TIM1, HighLevelTime);
}

void SetServo1_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH2(TIM1, HighLevelTime);
}

void SetServo2_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH3(TIM1, HighLevelTime);
}

void SetServo3_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH4(TIM1, HighLevelTime);
}

void SetServo4_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH1(TIM2, HighLevelTime);
}

void SetServo5_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH2(TIM2, HighLevelTime);
}

#else

/**
 * @brief: Set the high level time of Servos
 * @param: High Level Time of Servo, the unit is 0.01 ms
 * 		ParamRange: 50~250
 */
void ServoInit(void)
{
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH3);
	  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
	  LL_TIM_EnableAllOutputs(TIM1);
	  LL_TIM_EnableCounter(TIM1);
}

/**
 * @group: API for setting high level times for all servos.
 */
void SetServo0_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH1(TIM1, HighLevelTime);
}

void SetServo1_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH2(TIM1, HighLevelTime);
}

void SetServo2_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH3(TIM1, HighLevelTime);
}

void SetServo3_Time(uint16_t HighLevelTime)
{
	LL_TIM_OC_SetCompareCH4(TIM1, HighLevelTime);
}

#endif
