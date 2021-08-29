/*
 * CatchTrashCan.c
 *
 *  Created on: 2021Äê8ÔÂ28ÈÕ
 *      Author: h13
 */
#include "CatchTrashCan.h"
#include "RobotConfigs.h"
//Arm Control
#include "ArmControl.h"
#include "Drivers.h"
#include "Ports.h"

void CatchTrashCan(void)
{
	ReleaseClaw();

	//Enter the ready position.
	uint16_t node0, node1, node2, node3;
	GetNodesAngle(&node0, &node1, &node2, &node3);

	if(node1 > 30)
	{
		while(node1 > 30)
		{
			node1 --;
			ArmNode1_Rotate(node1);
			SleepMillisecond(15);
		}
	} else {
		while(node1 < 30)
		{
			node1 ++;
			ArmNode1_Rotate(node1);
			SleepMillisecond(15);
		}
	}

	if(node2 < 100)
	{
		while(node2 < 100)
		{
			node2 ++;
			ArmNode2_Rotate(node2);
			SleepMillisecond(15);
		}
	} else {
		while(node2 > 100)
		{
			node2 --;
			ArmNode2_Rotate(node2);
			SleepMillisecond(15);
		}
	}

	while(node3 < 67.5)
	{
		node3 ++;
		ArmNode3_Rotate(node3);
		SleepMillisecond(15);
	}

	ClawRotate(90);

	SleepMillisecond(200);

	while(node1 < 95)
	{
		node1 ++;
		ArmNode1_Rotate(node1);
		SleepMillisecond(15);
	}

	SleepMillisecond(300);

	ClosureClaw();

	SleepMillisecond(500);

	while(node2 < 125)
	{
		node2 ++;
		ArmNode2_Rotate(node2);
		SleepMillisecond(15);
	}

	while(node1 > 60)
	{
		node1 --;
		ArmNode1_Rotate(node1);
		SleepMillisecond(15);
	}
}
