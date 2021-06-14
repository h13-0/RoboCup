/*
 * MotorPorts.c
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */
#include "MotorPorts.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_tim.h"
#endif

/**
 * @configs:Channel reverse settings.
 * @param:
 * 		SWITCH_CHANNELS:  Switch left and right channels.
 * 		REVERSE_CHANNEL1: Reverse H-bridge channel 1.
 * 		REVERSE_CHANNEL2: Reverse H-bridge channel 2.
 */
#define SWITCH_CHANNELS  0
#define REVERSE_CHANNEL1 0
#define REVERSE_CHANNEL2 1


/**
 * @brief: Init Motor of the robot.
 */
void MotorInit(void) {
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableCounter(TIM3);
}


/**
 * @group: Set PWM Value Ports
 * @param:
 * 		value: Value of Channels
 */

//The TIM channel accessed by the H-bridge
static TIM_TypeDef *timTarget = TIM3;

/**
 * @brief: Set Value Of Channels
 * @param:
 * 		value: Value of Channels(Absolute value is less than maxPWMValue)
 */
#if SWITCH_CHANNELS
#if REVERSE_CHANNEL1

__attribute__((always_inline)) inline void setChannel1Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH4(timTarget, value);
}

__attribute__((always_inline)) inline void setChannel2Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH3(timTarget, value);
}

#else

__attribute__((always_inline)) inline void setChannel1Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH3(timTarget, value);
}

__attribute__((always_inline)) inline void setChannel2Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH4(timTarget, value);
}

#endif

#if REVERSE_CHANNEL2

__attribute__((always_inline)) inline void setChannel3Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH2(timTarget, value);
}


__attribute__((always_inline)) inline void setChannel4Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH1(timTarget, value);
}

#else

__attribute__((always_inline)) inline void setChannel3Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH1(timTarget, value);
}


__attribute__((always_inline)) inline void setChannel4Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH2(timTarget, value);
}

#endif

#else

#if REVERSE_CHANNEL1

__attribute__((always_inline)) inline void setChannel1Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH2(timTarget, value);
}

__attribute__((always_inline)) inline void setChannel2Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH3(timTarget, value);
}

#else

__attribute__((always_inline)) inline void setChannel1Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH1(timTarget, value);
}

__attribute__((always_inline)) inline void setChannel2Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH2(timTarget, value);
}

#endif //REVERSE_CHANNEL1

#if REVERSE_CHANNEL2

__attribute__((always_inline)) inline void setChannel3Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH4(timTarget, value);
}

__attribute__((always_inline)) inline void setChannel4Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH3(timTarget, value);
}

#else

__attribute__((always_inline)) inline void setChannel3Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH3(timTarget, value);
}

__attribute__((always_inline)) inline void setChannel4Value(uint16_t value) {
	LL_TIM_OC_SetCompareCH4(timTarget, value);
}

#endif //REVERSE_CHANNEL2

#endif
