/*
 * ports.c
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */

#include <stdint.h>

#include "mpu9250.h"
#include "inv_mpu.h"

extern I2C_HandleTypeDef hi2c1;

/**
 * @brief:  Initialize the gyroscope.
 * @return: Error code, success is 0.
 * @note:
 * 		9-axis or 10-axis gyroscopes are recommended.
 * 		MPU9250 is used here, **If you use a different type of gyroscope, delete the MPU9250 folder.**
 */
uint8_t MPUInit(void)
{
	return MPU9250_Init(&hi2c1);
}

/**
 * @brief:  Initialize the DMP(Digital Motion Processer) of the gyroscope.
 * @return: Error code, success is 0.
 */
uint8_t DMPInit(void)
{
	return mpu_dmp_init();
}

/**
 * @brief:  Obtain the euler Angle of gyroscope.
 * @param:  Pointers of the euler Angle.
 * @return: Error code, success is 0.
 */
uint8_t MPUGetData(float *pitch, float *roll, float *yaw)
{
	return mpu_mpl_get_data(pitch, roll, yaw);
}
