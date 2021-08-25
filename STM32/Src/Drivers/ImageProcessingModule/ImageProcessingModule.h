/*
 * ImageProcessingModule.h
 *
 *  Created on: 2021Äê8ÔÂ10ÈÕ
 *      Author: h13
 */

#ifndef DRIVERS_IMAGEPROCESSINGMODULE_IMAGEPROCESSINGMODULE_H_
#define DRIVERS_IMAGEPROCESSINGMODULE_IMAGEPROCESSINGMODULE_H_

#include <stdint.h>
#include "ports.h"

/**
 * @brief: Image processing module serial handler.
 * @param: data.
 */
void ImageProcessingModuleHandler(uint8_t data);

typedef enum
{
	NotReady = -1,
	Standby  = 0,
	AppleDetectMax = 1,
	AppleDetectLeft = 2,
	AppleDetectRight = 3,
	TargetDetect = 4,
	FruitDetection = 5,
} ImageProcessingModuleWorkingMode_t;

/**
 * @brief: Switch the working mode of image processing module.
 * @param: Working mode.
 * @note:  Invalid when Mode is equal to NotReady.
 */
void SwitchImageProcessingModuleWorkingMode(ImageProcessingModuleWorkingMode_t Mode);

/**
 * @brief:  Get the current mode of image processing module.
 * @return: ImageProcessingModuleWorkingMode_t.
 */
ImageProcessingModuleWorkingMode_t GetImageProcessingModuleWorkingMode(void);

/**
 * @brief: Typedef struct of coordinates.
 * @param:
 * 		float X: range: [0, 1]
 * 		float Y: range: [0, 1]
 * 		mtime_t TimeStamp: Time stamp in millisecond.
 */
typedef struct
{
	float X;
	float Y;
	mtime_t TimeStamp;
} Coordinates_t;

/**
 * @brief: Get the coordinates of apple.
 * @param: Pointer of coordinates.
 */
void GetAppleCoordinates(Coordinates_t *Coordinates);

void GetTargetCoordinates(Coordinates_t *Coordinates);

#endif /* DRIVERS_IMAGEPROCESSINGMODULE_IMAGEPROCESSINGMODULE_H_ */
