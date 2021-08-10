/*
 * App.c
 *
 *  Created on: 2021��5��29��
 *      Author: h13
 */
#include "App.h"

#include <stdio.h>

//Drivers
#include "ports.h"

#include "Drivers.h"
//App
//LVGL
#include "lvgl.h"
//Motion Control
#include "MotionControl.h"

//Arm Control
#include "ArmControl.h"

int App(void) {
	//Init Clock First.
	ClockInit();

	//Init Log.
	LogInit();

	Log(Debug, "Start!");

	//First run
#ifdef FirstRun
	//Init WT101
	SetWT101_ReturnRateTo(Rate100Hz);

	//Please close the macro definition and rebuild and redownload the program.
	while(1)
		Log(Info, "All peripherals are configured, Please close the macro definition and rebuild and redownload the program.");
#endif

	ResetYaw();

	MotorInit();
	MotionControlInit();
	ServoInit();

	//CalibrationAllServo();

	ArmNode3_Rotate(30);
	//while(1);

	//SetClawPosition(90, 0, 255);  //255 -> 250

	while (1)
	{
		//float data[]={ GetTOF_Distance(), GetYawValue()};
		//LogJustFloat(data, 2);
	}


/**Display Test
	SPIInit();
	LCDInit();

	LCDClear();

	while (1) {
		FillScreen(RED);
		FillScreen(GREEN);
		FillScreen(BLUE);
	}
*/

/**LVGL Test
	SPIInit();
	lv_init();
	lv_port_disp_init();


	while(1)
	{
		lv_tick_inc(10);
		lv_task_handler();
		HAL_Delay(10);
	}
*/
}
