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

int App(void) {
	//Init Log First.
	LogInit();

	Log(Debug, "Start!");

	//First run
#ifdef FirstRun
	//Init WT101
	SetWT101ReturnRateTo100();

	//Please close the macro definition and rebuild and redownload the program.
	while(1)
		Log(Info, "All peripherals are configured, Please close the macro definition and rebuild and redownload the program.");
#endif

	ResetYaw();

	MotorInit();
	MotionControlInit();

	KeepAngle();
	//KeepDistance(500);

	StraightUntill(300);

	TurnTo(Left);

	StraightUntill(1775);

	TurnTo(Right);

	StraightUntill(325);

	TurnTo(Left);

	StraightUntill(300);

	TurnTo(Right);

	TurnTo(Left);

	StraightUntill(3700);

	StraightUntill(2400);

	TurnTo(Right);

	StraightUntill(2400);

	TurnTo(Right);

	StraightUntill(650);

	TurnTo(Left);

	StraightUntill(2200);

	StraightUntill(300);

	TurnTo(Left);

	StraightUntill(400);

	TurnTo(Right);

	StraightUntill(2100);

	//while(1);


	while (1)
	{
		float data[]={GetTOF_Distance()};
		LogJustFloat(data, 1);
	}


/*
	SPIInit();
	LCDInit();

	LCDClear();

	while (1) {
		FillScreen(RED);
		FillScreen(GREEN);
		FillScreen(BLUE);
	}
*/
/*
	SPIInit();
	lv_init();
	lv_port_disp_init();


	while(1)
	{
		lv_tick_inc(10);
		lv_task_handler();
		HAL_Delay(10);
		float pitch = 0, roll = 0, yaw = 0;
		MPUGetData(&pitch, &roll, &yaw);
		float data[] = {pitch, roll, yaw};
		LogJustFloat(data, 3);
	}
*/
}