/*
 * Timer.c
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */
#include "Timer.h"

#include "MotionControl.h"
#include "ArmControl.h"

/**
 * @brief: Timer Interrupt Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
__attribute__((always_inline)) inline void TimerHandler(void)
{
	DirectionPIDCalculateHandler();
	ForwardPIDCalculateHandler();

	static uint8_t interruptCounter50hz = 0;
	if(interruptCounter50hz)
	{
		interruptCounter50hz = 0;
		ArmControlPIDCalculateHandler();
	} else {
		interruptCounter50hz = 1;
	}
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
