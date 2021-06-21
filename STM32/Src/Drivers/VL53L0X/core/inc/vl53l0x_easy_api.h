#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_it.h"

//使能2.8V IO电平模式
#define USE_I2C_2V8  1

/**
 * @groups: Apis
 */

/**
 * @brief: Init VL53L0X
 * @param:
 * 		I2C_HandleTypeDef *port:      I2C Port.
 *		VL53L0X_Dev_t     *dev:       Pointer of vl53l10x.
 *		GPIO_TypeDef      *XshutPort: GPIO Port of Xshut.
 *		uint32_t          XshutPin:   GPIO Pin of Xshut.
 *
 * @return: Result.
 * @note:
 * 		This function will reset your vl53l0x to facilitate the use of multiple devices.
 * 		If you need to use more than one vl53l0x, manually change the device address to another appropriate value after initialization.
 */
VL53L0X_Error VL53l0X_Init(I2C_HandleTypeDef *port , VL53L0X_Dev_t *dev, GPIO_TypeDef *XshutPort, uint32_t XshutPin);

/**
 * @brief: Set the address of VL53L0X.
 * @param:
 * 		VL53L0X_Dev_t *dev:    Pointer of vl53l10x.
 * 		uint8_t       newAddr: New address.
 * @return: Result.
 */
VL53L0X_Error VL53l0X_SetAddr(VL53L0X_Dev_t *dev, uint8_t newAddr);

/**
 * @brief: Some preset working modes, if you want to customize the mode, please modify the corresponding code.
 */
typedef enum
{
	Default_Mode  = 0,
	High_Accuracy = 1,
	Long_Range    = 2,
	High_Speed    = 3,
} VL5310X_Mode_t;

/**
 * @brief: Set VL5310X Mode.
 * @param:
 * 		VL53L0X_Dev_t *dev:  Pointer of vl53l10x.
 * 		VL5310X_Mode_t mode: mode.
 * @return: Result.
 */
VL53L0X_Error VL53l0X_SetMode(VL53L0X_Dev_t *dev, VL5310X_Mode_t mode);

/**
 * @brief: Vl53l0x single measurement distance.
 * @param:
 * 		VL53L0X_Dev_t *dev:  Pointer of vl53l10x.
 * 		uint16_t *pDistance: Pointer of distance in millimeter.
 */
VL53L0X_Error VL53l0X_Ranging(VL53L0X_Dev_t *dev, uint16_t *pDistance);

//Adjust
#define adjustRetryLimit 5//校准错误次数

/**
 * @brief: Adjust VL53L0X.
 * @param: Pointer of vl53l10x.
 * @return: Result.
 * @note:
 * 		After calibration, the calibration variable results will be output from printf.
 * 		Copy output to `void vl53l0x_adjust_init()` in `vl53l0x_easy_api.c`.
 * 		Recalibration may be required when the temperature difference is greater than 8 degrees.
 */
VL53L0X_Error VL53l0X_Adjust(VL53L0X_Dev_t *dev);

#endif
