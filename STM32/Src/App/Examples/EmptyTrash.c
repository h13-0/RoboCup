/*
 * EmptyTrash.c
 *
 *  Created on: 2021Äê8ÔÂ28ÈÕ
 *      Author: h13
 */
#include "EmptyTrash.h"
//Arm Control
#include "ArmControl.h"
#include "Drivers.h"
#include "Ports.h"

void EmptyTrash(void)
{
	SleepMillisecond(200);

	float node0, node1, node2, node3;
	GetNodesAngle(&node0, &node1, &node2, &node3);
	float originalNode3 = node3, originalNode2 = node2;

	while(node3 > 0)
	{
		node3 --;
		ArmNode3_Rotate(node3);
		SleepMillisecond(25);
	}

	while(node2 < 180)
	{
		node2 ++;
		ArmNode2_Rotate(node2);
		SleepMillisecond(25);
	}

	SleepMillisecond(500);

	while(node3 < originalNode3)
	{
		node3 ++;
		ArmNode3_Rotate(node3);
		SleepMillisecond(25);
	}

	while(node2 > originalNode2)
	{
		node2 --;
		ArmNode2_Rotate(node2);
		SleepMillisecond(25);
	}
}
