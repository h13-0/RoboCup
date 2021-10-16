/*
 * TimerInterrupts.c
 *
 *  Created on: 2021Äê6ÔÂ17ÈÕ
 *      Author: h13
 */

#include "TimerInterrupts.h"

#include "Timer.h"

/**
 * @brief: Timer Interrupt Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
__attribute__((always_inline)) inline void TimerInterruptHandler(void)
{
	TimerHandler();
}

/**
 * @brief: Timer for LVGL and Voice queue.
 * @note:
 * 		**Call frequency: 30Hz.**
 */
__attribute__((always_inline)) inline void UI_VoiceInterruptHandler(void)
{
	UI_VoiceTimerHandler();
}

#if(ArmType == LiftingPlatform)

/**
 * @brief: Timer for Stepper motor pulse.
 * @note:
 * 		**Call frequency: 2000Hz.**
 */
__attribute__((always_inline)) inline void StepperMotorTimerInterruptHandler(void)
{
	StepperTimerInterruptHandler();
}

#endif

