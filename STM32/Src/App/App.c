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

int App(void)
{
	//Init all modules and peripherals.
	//Current posture: ¡ý
	AllInit();

	ConnectToBluetoothDevice();

	while(1);

	ClosureClaw();

	PlaceApple();

	SleepMillisecond(3000);

	CatchApple(RightTarget);

	while(1);

	//Go to the refrigerator.
	StraightUntill(325 + 150);

	TurnTo(Left);

	StraightUntill(375 + 150);

	TurnTo(Right);

	StraightUntill(1600);

	TurnTo(Left);

	StraightUntill(600);

	TurnTo(Left);

	StraightUntill(1550);

	//Arrival refrigerator.
	//Current posture: ¡ü
	TurnOnTheBluetoothLight();

	//TODO: Identify fruit.
	//Catch apple.
	CatchApple(MaximumTarget);

	//Go to the pool.
	StraightUntill(400);

	TurnTo(Left);

	StraightUntill(650);

	TurnTo(Left);

	StraightUntill(1350);

	//Arrival pool.
	//Current posture: ¡ý
	WashApple();

	//Go to the desk.


}
