/*
 * SystemInit.c
 *
 *  Created on: 2021Äê8ÔÂ22ÈÕ
 *      Author: h13
 */

#include <AllInit.h>
#include "ArmControl.h"
#include "ports.h"

#include "Drivers.h"
//App
#include "Examples.h"
//LVGL
#include "lvgl.h"
//Motion Control
#include "MotionControl.h"

//Arm Control

/**
 * @brief: Init all modules and peripherals.
 */
void AllInit(void)
{
	//Init all base peripherals.
	PortsInit();

	//Init Log.
	LogInit();

	Log(Debug, "Start!");

	BluetoothInit();

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
}
