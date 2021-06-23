/*
 * TimerInterrupts.h
 *
 *  Created on: 2021Äê6ÔÂ17ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_INTERRUPTS_TIMERINTERRUPTS_TIMERINTERRUPTS_H_
#define PLATFORM_INTERRUPTS_TIMERINTERRUPTS_TIMERINTERRUPTS_H_

/**
 * @brief: Timer Interrupt Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
void TimerInterruptHandler(void);

/**
 * @brief: Timer for LVGL.
 * @note:
 * 		Responsible for refreshing UI.
 * 		**Call frequency: 30Hz.**
 */
void LVGLInterruptHandler(void);

#endif /* PLATFORM_INTERRUPTS_TIMERINTERRUPTS_TIMERINTERRUPTS_H_ */
