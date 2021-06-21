/*
 * App.c
 *
 *  Created on: 2021Äê5ÔÂ29ÈÕ
 *      Author: h13
 */
#include "App.h"

#include <stdio.h>

//Drivers
#include "ports.h"

#include "Drivers.h"
//App
//LVGL
#include "lvgl.h"
//Motion Control
#include "MotionControl.h"

int App(void) {
	//Init Log First.
	LogInit();
	Log(Debug, "Start!");

	 //Init MPU and DMP.
	 uint8_t ret = 0;
	 //Init MPU
	 ret = MPU_Init();
	 while (ret) {
		 Log(Warning, "MPU Init Fail with ret: %d", ret);
		 DelayMS(400);
		 ret = MPU_Init();
	 }

	 Log(Info, "MPU OK");

	 //Init DMP
	 ret = DMP_Init();
	 while (ret) {
		 Log(Warning, "dmp Init Fail with ret: %d", ret);
		 DelayMS(400);
		 ret = DMP_Init();
	 }

	 Log(Info, "dmp OK");

	 MotorInit();
	 MotionControlInit();

	 KeepAngle(0);

	 while(1);

/*
	 while (1)
	 {
		 float data[]={GetTOFDistance()};
		 SendJustFloatFrame(data, 1);
	 }
*/

/*
	SPIInit();
	LCDInit();

	LCDClear();

	while (1) {
		FillScreen(RED);
		FillScreen(GREEN);
		FillScreen(BLUE);
	}
*/
/*
	SPIInit();
	lv_init();
	lv_port_disp_init();


	while(1)
	{
		lv_tick_inc(10);
		lv_task_handler();
		HAL_Delay(10);
		float pitch = 0, roll = 0, yaw = 0;
		MPUGetData(&pitch, &roll, &yaw);
		float data[] = {pitch, roll, yaw};
		LogJustFloat(data, 3);
	}
*/
}
