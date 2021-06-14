/*
 * Timer.c
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */
#include "Timer.h"

#include "PIDManagement.h"

/**
 * @brief: Timer Interrupt Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
__attribute__((always_inline)) inline void TimerHandler(void)
{
	PIDHandler();
}

/**
 * @brief: Timer for LVGL.
 * @note:
 * 		Responsible for refreshing UI.
 * 		**Call frequency: 30Hz.**
 */
__attribute__((always_inline)) inline void LVGLHandler(void)
{

}

