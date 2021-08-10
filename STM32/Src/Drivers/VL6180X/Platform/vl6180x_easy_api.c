/*
 * VL6180x_easy_api.c
 *
 *  Created on: 2021Äê7ÔÂ19ÈÕ
 *      Author: h13
 */

#include "VL6180x_easy_api.h"

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
void VL6180x_Init(I2C_HandleTypeDef *port , VL6180x_Dev_t *dev, GPIO_t XshutIO)
{
	dev -> I2cAddr = 0x52;

	dev -> XshutIO = XshutIO;

	VL6180x_Shutdown(dev);
}

/**
 * @brief: Enable device.
 * @note:
 * 		You should use this function after initializing the device.
 */
int VL6180x_Enable(VL6180x_Dev_t *dev)
{
	VL6180x_StartUp(dev);
	uint8_t addr;
	addr = dev -> I2cAddr;     //Save Addr
	dev -> I2cAddr = 0x52;

	int ret = 0;

	ret = VL6180x_InitData(dev);
	if(ret)
		return ret;

	ret = VL6180x_Prepare(dev);
	if(ret)
		return ret;

	ret = VL6180x_SetI2CAddress(dev, addr);  //Load Addr
	if(ret)
		return ret;

	return 0;
}

/**
 * @brief: Disable device.
 */
void VL6180x_Disable(VL6180x_Dev_t *dev)
{
	VL6180x_Shutdown(dev);
}

/**
 * @brief: Set the address of VL6180x.
 * @param:
 * 		VL6180x_Dev_t *dev:    Pointer of vl53l10x.
 * 		uint8_t       newAddr: New address.
 * @return: Result.
 */
int VL6180x_SetAddr(VL6180x_Dev_t *dev, uint8_t newAddr)
{
	int ret = 0;

	if(newAddr == dev -> I2cAddr)
		return 0;

	ret = VL6180x_SetI2CAddress(dev, newAddr);
	if(ret)
		return ret;

	dev -> I2cAddr = newAddr;

	return 0;
}

/**
 * @brief: VL6180x single measurement distance.
 * @param:
 * 		VL6180x_Dev_t *dev:  Pointer of vl53l10x.
 * 		uint16_t *pDistance: Pointer of distance in millimeter.
 */
uint32_t VL6180x_Ranging(VL6180x_Dev_t *dev, int32_t *pDistance)
{
	int ret = 0;
	VL6180x_RangeData_t tempData;
	ret = VL6180x_RangePollMeasurement(dev, &tempData);
	if(ret)
	{
		return tempData.errorStatus;
	} else {
		*pDistance = tempData.range_mm;
	}
	return 0;
}
