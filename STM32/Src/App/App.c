/*
 * App.c
 *
 *  Created on: 2021Äê5ÔÂ29ÈÕ
 *      Author: h13
 */
#include "App.h"
#include "RobotConfigs.h"
#include "Examples.h"

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
	//Init all base peripherals.
	PortsInit();

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

	MotionControlInit();

	ArmControlInit();

	CatchApple();

	while (1)
	{
		Coordinates_t coordinates = { 0 };
		GetAppleCoordinates(&coordinates);
		float data[] = { coordinates.X, coordinates.Y, coordinates.TimeStamp };
		LogJustFloat(data, 3);
		//float data[]={ GetTOF_Distance(), GetYawValue()};
		//LogJustFloat(data, 2);
	}
}
