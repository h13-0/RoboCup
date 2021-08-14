/*
 * ports.h
 *
 *  Created on: 2021Äê5ÔÂ29ÈÕ
 *      Author: h13
 *      Team: Spark, cqjtu.
 */

#ifndef PLATFORM_PORTS_H_
#define PLATFORM_PORTS_H_

#include <stdint.h>

/**
 * @group: Basic APIs.
 * @note:  The following include files **also need to be migrated**.
 */
#include "Clock.h"
#include "AppLog.h"
#include "SPI.h"
#include "I2C.h"

/**
 * @group: Additional APIs.
 */
//WT101
#include "WT101Ports.h"
//Debug
#include "DebugPorts.h"

//Servo
#include "ServoPorts.h"
#include "Servo.h"

//Motor
#include "MotorPorts.h"
#include "Motor.h"

//Image Processing Module
#include "ImageProcessingModule.h"

/**
 * @brief: Initialize all base peripherals.
 */
void PortsInit(void);

#endif /* APP_PORTS_H_ */
