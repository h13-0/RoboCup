/*
 * TimerInterrupts.h
 *
 *  Created on: 2021Äê6ÔÂ17ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_INTERRUPTS_TIMERINTERRUPTS_TIMERINTERRUPTS_H_
#define PLATFORM_INTERRUPTS_TIMERINTERRUPTS_TIMERINTERRUPTS_H_

#include "RobotConfigs.h"

/**
 * @brief: Timer Interrupt Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
void TimerInterruptHandler(void);

/**
 * @brief: Timer for LVGL and Voice queue.
 * @note:
 * 		**Call frequency: 30Hz.**
 */
void UI_VoiceInterruptHandler(void);

#if(ArmType == LiftingPlatform)

/**
 * @brief: Timer for Stepper motor pulse.
 * @note:
 * 		**Call frequency: 2000Hz.**
 */
void StepperMotorTimerInterruptHandler(void);

#endif

#endif /* PLATFORM_INTERRUPTS_TIMERINTERRUPTS_TIMERINTERRUPTS_H_ */
