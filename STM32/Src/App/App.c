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
//Voice player
#include "VoicePlayer.h"

int App(void)
{
	//Init all modules and peripherals.
	//Current posture: ¡ý
	AllInit();

	//Voice_t voices[] = { BroadcastFruitDetectResult, BroadcastApple, BroadcastOne };
	//VoicePlayerPlayByIDS(voices, 3);

	//CalibrationAllServo();

	ConnectToBluetoothDevice();

	while(1)
	{
		FillScreen(RED);
		FillScreen(GREEN);
		FillScreen(BLUE);
	}

/*
	while(1)
	{
		float data = GetTOF_Distance();
		LogJustFloat(&data, 1);
	}
*/

	//while(1);
	//KeepSpeed();
	//KeepAngle();
	//KeepDistance(500);

	//Go to the refrigerator.
	StraightUntill(325 + 150);

	TurnTo(Left);

	StraightUntill(1750 - 150);

	TurnTo(Right);

	StraightUntill(1500);

	TurnTo(Left);

	StraightUntill(DisktopDistanceBeforeTurn - 50);

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

	StraightUntill(DisktopDistanceBeforeTurn);

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

	StraightUntill(DisktopDistanceBeforeTurn);

	TurnTo(Left);

	StraightUntill(1950);

	//Arrival Desktop.
	//Current posture: ¡ý
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

	StraightUntill(500);

	//EmptyTrash();

	TurnTo(Left);

	StraightUntill(1650);

	//PlaceTrashCan();

	TurnTo(Left);

	StraightUntill(2450);

	while(1);
}
