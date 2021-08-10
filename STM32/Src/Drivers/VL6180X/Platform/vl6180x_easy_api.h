/*
 * vl6180x_easy_api.h
 *
 *  Created on: 2021Äê7ÔÂ19ÈÕ
 *      Author: h13
 */

#ifndef SRC_VL6180X_PLATFORM_VL6180X_EASY_API_H_
#define SRC_VL6180X_PLATFORM_VL6180X_EASY_API_H_

#include "vl6180x_api.h"

/**
 * @groups: Apis
 */

/**
 * @brief: Init VL6180x **and shutdown it.**
 * @param:
 * 		I2C_HandleTypeDef *port:      I2C Port.
 *		VL6180x_Dev_t     *dev:       Pointer of vl53l10x.
 *		GPIO_TypeDef      *XshutPort: GPIO Port of Xshut.
 *		uint32_t          XshutPin:   GPIO Pin of Xshut.
 *
 * @return: Result.
 * @note:
 * 		This function will initialize the VL6180x_Dev_t structure. Then shut down the device.
 * 		**You need to manually enable the device after this using the "VL6180x_Enable" function.**
 * 		**If you use multiple devices,
 * 			use this function to initialize all the structures and shut down the devices before manually assigning each device in turn to a new address**
 *
 */
void VL6180x_Init(I2C_HandleTypeDef *port , VL6180x_Dev_t *dev, GPIO_t XshutIO);

/**
 * @brief: Enable device.
 * @note:
 * 		You should use this function after initializing the device.
 */
int VL6180x_Enable(VL6180x_Dev_t *dev);
/**
 * @brief: Disable device.
 */
void VL6180x_Disable(VL6180x_Dev_t *dev);

/**
 * @brief: Set the address of VL6180X.
 * @param:
 * 		VL6180X_Dev_t *dev:    Pointer of vl53l10x.
 * 		uint8_t       newAddr: New address.
 * @return: Result.
 */
int VL6180x_SetAddr(VL6180x_Dev_t *dev, uint8_t newAddr);

/**
 * @brief: Some preset working modes, if you want to customize the mode, please modify the corresponding code.
 */
typedef enum
{
	Default_Mode  = 0,
	High_Accuracy = 1,
	Long_Range    = 2,
	High_Speed    = 3,
} VL6180X_Mode_t;

/**
 * @brief: VL6180X single measurement distance.
 * @param:
 * 		VL6180X_Dev_t *dev:  Pointer of vl53l10x.
 * 		uint16_t *pDistance: Pointer of distance in millimeter.
 */
uint32_t VL6180x_Ranging(VL6180x_Dev_t *dev, int32_t *pDistance);


#endif /* SRC_VL6180X_PLATFORM_VL6180X_EASY_API_H_ */
