/*
 * Timer.c
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */

#include "RobotConfigs.h"
#include "ArmControl.h"
#include "Timer.h"

#include "MotionControl.h"
#include "Stepper.h"

/**
 * @brief: Timer Interrupt Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
__attribute__((always_inline)) inline void TimerHandler(void)
{
	SpeedPIDCalculateHandler();
	DirectionPIDCalculateHandler();
	ForwardPIDCalculateHandler();
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

#if(ArmType == LiftingPlatform)

/**
 * @brief: Timer for Stepper motor pulse.
 * @note:
 * 		**Call frequency: 2000Hz.**
 */
__attribute__((always_inline)) inline void StepperTimerInterruptHandler(void)
{
	SteperTimerHandler();
}

#endif

