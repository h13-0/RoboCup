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
	 ret = MPUInit();
	 while (ret) {
		 Log(Warning, "MPU Init Fail with ret: %d", ret);
		 DelayMS(400);
		 ret = MPUInit();
	 }

	 Log(Info, "MPU OK");

	 //Init DMP
	 ret = DMPInit();
	 while (ret) {
		 Log(Warning, "dmp Init Fail with ret: %d", ret);
		 DelayMS(400);
		 ret = DMPInit();
	 }

	 Log(Info, "dmp OK");

	 MotorInit();
	 MotionControlInit();

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
	uint8_t ret = 0;
		 //Init MPU
		 ret = MPUInit();
		 while (ret) {
		 char logMsg[32];
		 sprintf(logMsg, "MPU Init Fail with ret: %d", ret);
		 Log(Warning, logMsg);
		 DelayMS(400);
		 ret = MPUInit();
		 }

		 Log(Info, "MPU OK");

		 //Init DMP
		 ret = DMPInit();
		 while (ret) {
		 char logMsg[32];
		 sprintf(logMsg, "dmp Init Fail with ret: %d", ret);
		 Log(Warning, logMsg);
		 DelayMS(400);
		 ret = DMPInit();
		 }

		 Log(Info, "dmp OK");

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

	/*
	 uint8_t ret = 0;
	 //Init MPU
	 ret = MPUInit();
	 while (ret) {
	 char logMsg[32];
	 sprintf(logMsg, "MPU Init Fail with ret: %d", ret);
	 Log(Warning, logMsg);
	 DelayMS(400);
	 ret = MPUInit();
	 }

	 Log(Info, "MPU OK");

	 //Init DMP
	 ret = DMPInit();
	 while (ret) {
	 char logMsg[32];
	 sprintf(logMsg, "dmp Init Fail with ret: %d", ret);
	 Log(Warning, logMsg);
	 DelayMS(400);
	 ret = DMPInit();
	 }

	 Log(Info, "dmp OK");

	 while(1)
	 {
	 float pitch, roll, yaw;
	 MPUGetData(&pitch, &roll, &yaw);
	 float data[]={pitch, roll, yaw};
	 LogJustFloat(data, 3);
	 }
	 */

}
