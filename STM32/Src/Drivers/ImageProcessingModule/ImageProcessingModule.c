/*
 * ImageProcessingModule.c
 *
 *  Created on: 2021Äê8ÔÂ10ÈÕ
 *      Author: h13
 */
#include "ImageProcessingModule.h"
#include "ports.h"
#include "RobotConfigs.h"
#include "SimpleProtocolPraise.h"
#include "FastMatch.h"

#include <stdint.h>

static ImageProcessingModuleWorkingMode_t currentMode = NotReady;
static Coordinates_t appleCoordinates = { 0 };
static Coordinates_t targetCoordinates = { 0 };

static int8_t fruitDetectResult[9] = { -1 };

static void updateAppleTimeStamp(void)
{
	appleCoordinates.TimeStamp = GetCurrentTimeMillisecond();
}

static void updateTargetTimeStamp(void)
{
	targetCoordinates.TimeStamp = GetCurrentTimeMillisecond();
}

static void binaryProtocolPraise(char *data, uint8_t length)
{
	MatchKeyInt8_t(data, length, "WM:", 3, currentMode, return);

	//AppleDetect
	MatchKeyFloat(data, length, "AppCenX:", 8, appleCoordinates.X, updateAppleTimeStamp(); return);
	MatchKeyFloat(data, length, "AppCenY:", 8, appleCoordinates.Y, updateAppleTimeStamp(); return);

	//TargetDetect
	MatchKeyFloat(data, length, "TarCenX:", 8, targetCoordinates.X, updateTargetTimeStamp(); return);
	MatchKeyFloat(data, length, "TarCenY:", 8, targetCoordinates.Y, updateTargetTimeStamp(); return);

	//FruitDetect
	MatchKeyInt8_t(data, length, "AppleNum:", 9, fruitDetectResult[FruitApple], return);
	MatchKeyInt8_t(data, length, "BananaNum:", 10, fruitDetectResult[FruitBanana], return);
	MatchKeyInt8_t(data, length, "KiwiFruitNum:", 13, fruitDetectResult[FruitKiwiFruit], return);
	MatchKeyInt8_t(data, length, "LemonNum:", 9, fruitDetectResult[FruitLemon], return);
	MatchKeyInt8_t(data, length, "OrangeNum:", 10, fruitDetectResult[FruitOrange], return);
	MatchKeyInt8_t(data, length, "PeachNum:", 9, fruitDetectResult[FruitPeach], return);
	MatchKeyInt8_t(data, length, "PearNum:", 8, fruitDetectResult[FruitPear], return);
	MatchKeyInt8_t(data, length, "PitayaNum:", 10, fruitDetectResult[FruitPitaya], return);
	MatchKeyInt8_t(data, length, "SnowPearNum:", 12, fruitDetectResult[FruitSnowPear], return);
}

/**
 * @brief: Image processing module serial handler.
 * @param: data.
 */
__attribute__((always_inline)) inline void ImageProcessingModuleHandler(uint8_t data)
{
	static char buffer[ImageProcessingSerialBufferLength] = { '\0' };
	static uint8_t length = 0;
	static char* bufferPtr = buffer;
	GeneratePraiseWithSuffixMethod(data, "\r\n", 2, bufferPtr, ImageProcessingSerialBufferLength, length, binaryProtocolPraise(bufferPtr, length - 2));
}

/**
 * @brief: Init image processing module peripheral.
 */
void ImageProcessingModulePeripheral(void)
{
	SerialInit(ImageProcessingModulePort, ImageProcessingModulePortBaudRate);
}

__attribute__((always_inline)) static inline void imageProcessingModuleSendString(char *string)
{
	while(*string != '\0')
	{
		SerialSend(ImageProcessingModulePort, *string);
		string ++;
	}
}

/**
 * @brief: Switch the working mode of image processing module.
 * @param: Working mode.
 */
__attribute__((always_inline)) inline void SwitchImageProcessingModuleWorkingMode(ImageProcessingModuleWorkingMode_t Mode)
{
	while(currentMode != Mode)
	{
		//TODO: Timeout.
		switch(Mode)
		{
		case Standby:
			imageProcessingModuleSendString("CMD:Standby\r\n");
			break;

		case AppleDetectMax:
			imageProcessingModuleSendString("CMD:AppleDetectMax\r\n");
			break;

		case AppleDetectLeft:
			imageProcessingModuleSendString("CMD:AppleDetectLeft\r\n");
			break;

		case AppleDetectRight:
			imageProcessingModuleSendString("CMD:AppleDetectRight\r\n");
			break;

		case TargetDetect:
			imageProcessingModuleSendString("CMD:TargetDetect\r\n");
			break;

		case FruitDetection:
			imageProcessingModuleSendString("CMD:FruitDetect\r\n");
			break;

		default:
			break;
		}
		SleepMillisecond(50);
	}
}

/**
 * @brief:  Get the current mode of image processing module.
 * @return: ImageProcessingModuleWorkingMode_t.
 */
__attribute__((always_inline)) inline ImageProcessingModuleWorkingMode_t GetImageProcessingModuleWorkingMode(void)
{
	return currentMode;
}

/**
 * @brief: Get the coordinates of apple.
 * @param: Pointer of coordinates.
 */
__attribute__((always_inline)) inline void GetAppleCoordinates(Coordinates_t *Coordinates)
{
	*Coordinates = appleCoordinates;
}

/**
 * @brief: Get the coordinates of target.
 * @param: Pointer of coordinates.
 */
__attribute__((always_inline)) inline void GetTargetCoordinates(Coordinates_t *Coordinates)
{
	*Coordinates = targetCoordinates;
}

/**
 * @brief: Get the result of fruit detection.
 * @usage:
 * 	int8_t* result = GetFruitDetectionResult();
 * 	printf("AppleNumber: %d\r\n", *(result + FruitApple));
 */
__attribute__((always_inline)) inline int8_t* GetFruitDetectionResult(void)
{
	return fruitDetectResult;
}
