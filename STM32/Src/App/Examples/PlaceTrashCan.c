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
	ClawRotate(90);
	SleepMillisecond(200);

	float node0, node1, node2, node3;
	GetNodesAngle(&node0, &node1, &node2, &node3);

	while(node1 > 0)
	{
		node1 --;
		ArmNode1_Rotate(node1);
		SleepMillisecond(25);
	}

	if(node2 > 75)
	{
		while(node2 > 75)
		{
			node2 --;
			ArmNode2_Rotate(node2);
			SleepMillisecond(25);
		}
	} else {
		while(node2 < 75)
		{
			node2 ++;
			ArmNode2_Rotate(node2);
			SleepMillisecond(25);
		}
	}

	while(node3 < 90)
	{
		node3 ++;
		ArmNode3_Rotate(90);
		SleepMillisecond(25);
	}

	ReleaseClaw();

	SleepMillisecond(300);

	while(node2 < 120)
	{
		node2 ++;
		ArmNode2_Rotate(node2);
		SleepMillisecond(25);
	}

	ClosureClaw();

	ClawRotate(0);

	SleepMillisecond(200);

	while(node3 > 0)
	{
		ArmNode3_Rotate(-- node3);
		SleepMillisecond(25);
	}

	while(node2 > 0)
	{
		ArmNode2_Rotate(-- node2);
		SleepMillisecond(25);
	}

	while(node1 > 0)
	{
		ArmNode1_Rotate(-- node1);
		SleepMillisecond(25);
	}

	CalibrationAllServo();

	SetOpenLoopClawPosition(0, ArmNode2_Length - ArmNode1_Length + ArmNode3_Length, ArmNode0_Height);

	SleepMillisecond(250);
}

