/*
 * App.c
 *
 *  Created on: 2021Äê5ÔÂ29ÈÕ
 *      Author: h13
 */
#include "ArmControl.h"
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

int App(void)
{
	//Init all modules and peripherals.
	//Current posture: ¡ý
	AllInit();

	ConnectToBluetoothDevice();

	while(1);

	for(uint8_t i = 0; i < 10; i++)
	{
		TurnTo(Left);
		SleepMillisecond(1000);
	}

	for(uint8_t i = 0; i < 10; i++)
	{
		TurnTo(Right);
		SleepMillisecond(1000);
	}

	while(1);

	//KeepAngle();

	//KeepDistance(500);

	/*
	while(1);

	ClosureClaw();

	PlaceApple();

	SleepMillisecond(3000);

	CatchApple(RightTarget);

	while(1);
	*/

	//Go to the refrigerator.
	StraightUntill(325 + 150);

	TurnTo(Left);

	StraightUntill(1750 - 150);

	TurnTo(Right);

	StraightUntill(1600);

	TurnTo(Left);

	StraightUntill(500);

	TurnTo(Left);

	StraightUntill(1600 - 150);

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

	StraightUntill(450);

	//Arrival pool.
	//Current posture: ¡ý
	WashApple();

	//Go to the desk.
	StraightUntill(400);

	TurnTo(Right);

	StraightUntill(1750 - 150);

	TurnTo(Right);

	StraightUntill(1500);

	TurnTo(Left);

	StraightUntill(500);

	TurnTo(Left);

	StraightUntill(2150);

	while(1);

}
