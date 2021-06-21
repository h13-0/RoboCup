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


/**
 * @group: Additional APIs.
 */
//Servo
#include "ServoPorts.h"
#include "Servo.h"

//Motor
#include "MotorPorts.h"
#include "Motor.h"


/**
 * @brief:  Initialize the gyroscope.
 * @return: Error code, success is 0.
 * @note:
 * 		9-axis or 10-axis gyroscopes are recommended.
 * 		MPU9250 is used here, **If you use a different type of gyroscope, delete the MPU9250 folder.**
 */
uint8_t MPU_Init(void);

/**
 * @brief:  Initialize the DMP(Digital Motion Processer) of the gyroscope.
 * @return: Error code, success is 0.
 */
uint8_t DMP_Init(void);

/**
 * @brief:  Obtain the euler Angle of gyroscope.
 * @param:  Pointers of the euler Angle.
 * @return: Error code, success is 0.
 */
uint8_t MPU_GetData(float *pitch, float *roll, float *yaw);

#endif /* APP_PORTS_H_ */
