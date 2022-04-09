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

	SleepMillisecond(100000);
	while(1)
	{
		VoicePlayerSequentialPlay(BroadcastFruitDetectResult, BroadcastApple, BroadcastBanana, BroadcastKiwiFruit, BroadcastLemon, BroadcastOrange, BroadcastPeach);
		SleepMillisecond(10000);
	}

	/*
	char messageBuffer[128] = { 0 };
	LuatMessage_t message = LuatCreatNewMessage(messageBuffer, 128);
	LuatAddMessageContents(&message, UnicodeChsPing, UnicodeChsGuo);
	LuatSendMessage(&message, TargetPhone);

	//PrepareArmPosition(90);
	//PlaceApple();
	//CatchApple(CatchMaximumApple);

	SwitchImageProcessingModuleWorkingMode(FruitIdentify);
	while(1)
	{
		Coordinates_t coor;
		GetFruitsFocusCoordinates(&coor);
		LogJustFloat(coor.X, coor.Y, (float)(GetImageProcessingModuleWorkingMode()));
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

	StraightUntill(275);

	TurnTo(Right);

	StraightUntill(3820);

	//Arrival refrigerator.
	//Current posture: ¡ú
	TurnOnTheBluetoothLight();

	//Identify fruit.
	ReleaseClaw();
	PrepareArmPosition(180);
	IdentifyFruit();
	PrepareArmPosition(180);

	CatchApple(CatchMaximumApple);
	PrepareArmPosition(0);

	//Go to the pool.
	StraightUntill(150);

	//Arrival pool.
	//Current posture: ¡ú
	WashApple();

	//Go to the desk.
	StraightUntill(1200);

	TurnTo(Right);

	StraightUntill(1500);

	TurnTo(Left);

	StraightUntill(DisktopDistanceBeforeTurn);

	TurnTo(Left);

	StraightUntill(2170);

	//Arrival Desktop.
	//Current posture: ¡ý
	//Place apple.
	PrepareArmPosition(90);
	uint16_t angle = PlaceApple();

	//Catch another apple.
	PrepareArmPosition(90);
	ReleaseClaw();
	if(angle > 90)
	{
		CatchApple(CatchLeftApple);
	} else {
		CatchApple(CatchRightApple);
	}

	PrepareArmPosition(0);

	//Go to the trash can.
	//Current posture: ¡ý
	StraightUntill(325 + 150);

	TurnTo(Left);

	StraightUntill(1450);

	TurnTo(Right);

	StraightUntill(3515);

	//Arrive trash can.
	//Current posture: ¡ý
	ThrowApple();
	CatchTrashCan();

	StraightUntill(4350);

	TurnTo(Left);

	StraightUntill(1250);

	EmptyTrash();

	StraightUntill(500);

	PlaceTrashCan();

	TurnTo(Right);

	StraightUntill(2600);

	while(1);
}
