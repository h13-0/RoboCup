/*
 * PlaceTrashCan.c
 *
 *  Created on: 2021Äê8ÔÂ28ÈÕ
 *      Author: h13
 */
#include "PlaceTrashCan.h"
//Arm Control
#include "ArmControl.h"
#include "Drivers.h"
#include "Ports.h"

void PlaceTrashCan(void)
{
	SetArmNodeAngle(ClawRotation, 90);
	SleepMillisecond(200);

}
