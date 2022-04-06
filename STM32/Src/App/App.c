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

	//CatchTrashCan();
	//while(1);

	/*
	PrepareArmPosition(90);
	IdentifyFruit();

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

	StraightUntill(1500);

	TurnTo(Left);

	StraightUntill(DisktopDistanceBeforeTurn - 25);

	TurnTo(Left);

	StraightUntill(1600 - 300);

	//Arrival refrigerator.
	//Current posture: ¡ü
	TurnOnTheBluetoothLight();

	//Catch apple.
	ReleaseClaw();
	PrepareArmPosition(90);
	//TODO: Identify fruit.
	CatchApple(CatchMaximumApple);
	PrepareArmPosition(0);

	//Go to the pool.
	StraightUntill(400);

	TurnTo(Left);

	StraightUntill(DisktopDistanceBeforeTurn);

	TurnTo(Left);

	StraightUntill(450);

	//Arrival pool.
	//Current posture: ¡ý
	WashApple();

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
	PlaceApple();


	StraightUntill(2300);

	ReleaseClaw();
	CatchApple(CatchRightApple);
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

	StraightUntill(2500);

	TurnTo(Left);

	StraightUntill(650);

	TurnTo(Right);

	while(1);
}
