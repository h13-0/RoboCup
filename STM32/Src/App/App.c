/*
 * App.c
 *
 *  Created on: 2021Äê5ÔÂ29ÈÕ
 *      Author: h13
 */
#include "App.h"
#include "RobotConfigs.h"
#include "ArmControl.h"
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

	while(1);

	//KeepSpeed();

	//CalibrationAllServo();

	ConnectToBluetoothDevice();
/*
	while(1)
	{
		float data = GetTOF_Distance();
		LogJustFloat(&data, 1);
	}
*/
	KeepDistance(500);

	Stepper_t zAxisStepper = Z_AxisStepper;
	StepperInit(&zAxisStepper);

	while(1)
	{
		StepperForward(&zAxisStepper, 500);
		SleepMillisecond(1000);
		StepperBackward(&zAxisStepper, 500);
		SleepMillisecond(1000);
	}

	//while(1);
	//KeepSpeed();
	//KeepAngle();
	//KeepDistance(500);

	//Go to the refrigerator.
	StraightUntill(325 + 150);

	TurnTo(Left);

	StraightUntill(1750 - 150);

	TurnTo(Right);

	StraightUntill(1600);

	TurnTo(Left);

	StraightUntill(500);

	TurnTo(Left);

	StraightUntill(1600 - 300);

	//Arrival refrigerator.
	//Current posture: ¡ü
	TurnOnTheBluetoothLight();

	//TODO: Identify fruit.
	//Catch apple.
	//CatchApple(MaximumTarget);

	//Go to the pool.
	StraightUntill(400);

	TurnTo(Left);

	StraightUntill(500);

	TurnTo(Left);

	StraightUntill(450);

	//Arrival pool.
	//Current posture: ¡ý
	//WashApple();

	//Go to the desk.
	StraightUntill(1600);

	TurnTo(Right);

	StraightUntill(1750 - 150);

	TurnTo(Right);

	StraightUntill(1500);

	TurnTo(Left);

	StraightUntill(500);

	TurnTo(Left);

	StraightUntill(1950);

	//PlaceApple();

	StraightUntill(2300);

	//CatchApple(RightTarget);

	//Go to the trash can.
	//Current posture: ¡ý
	StraightUntill(325 + 150);

	TurnTo(Left);

	StraightUntill(2000 - 150);

	TurnTo(Right);

	StraightUntill(3100);

	//Arrive trash can.
	//Current posture: ¡ý
	//ThrowApple();

	//CatchTrashCan();

	SleepMillisecond(500);

	TurnTo(BackWard);

	StraightUntill(300);

	//EmptyTrash();

	TurnTo(Left);

	StraightUntill(1650);

	//PlaceTrashCan();

	TurnTo(Left);

	StraightUntill(2450);

	while(1);
}
