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

	ConnectToBluetoothDevice();

	PrepareArmPosition(90);
	IdentifyFruit();

	while(1)
	{
		Coordinates_t coor;
		GetTargetCoordinates(&coor);
		float data[] = { coor.X, coor.Y };
		LogJustFloat(data, 2);
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

	StraightUntill(1500);

	TurnTo(Left);

	StraightUntill(DisktopDistanceBeforeTurn - 25);

	TurnTo(Left);

	StraightUntill(1600 - 300);

	//Arrival refrigerator.
	//Current posture: ¡ü
	TurnOnTheBluetoothLight();

	//TODO: Identify fruit.
	//Catch apple.
	CatchApple(CatchMaximumApple);

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
