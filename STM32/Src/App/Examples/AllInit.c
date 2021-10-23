/*
 * SystemInit.c
 *
 *  Created on: 2021Äê8ÔÂ22ÈÕ
 *      Author: h13
 */

#include <AllInit.h>

#include "ports.h"

#include "Drivers.h"
//App
#include "Examples.h"

//Motion Control
#include "MotionControl.h"
//Arm Control
#include "ArmControl.h"
//Voice Player
#include "VoicePlayer.h"
#include "UI.h"

/**
 * @brief: Init all modules and peripherals.
 */
void AllInit(void)
{
	//Init all base peripherals.
	PortsInit();

	DriversInit();

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

	//Init application.
#if(ShowSchoolMotto)
	DisplaySchoolMotto();
#endif
	BluetoothInit();

	MotionControlInit();

	ArmControlInit();

	VoicePlayerInit();
}
