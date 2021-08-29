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
/*************************USART Ports************************/
//WT101
#include "WT101Ports.h"
//Debug
#include "DebugPorts.h"
//Image Processing Module
#include "ImageProcessingModulePorts.h"
//Bluetooth
#include "BluetoothPorts.h"

/**************************PWM Ports*************************/
#include "PWM.h"
//Servo
#include "ServoPorts.h"
#include "Servo.h"

/*************************GPIO Ports*************************/
#include "GPIO.h"
#include "HC05_GPIO_Ports.h"

/**
 * @brief: Initialize all base peripherals.
 */
void PortsInit(void);

#endif /* APP_PORTS_H_ */
