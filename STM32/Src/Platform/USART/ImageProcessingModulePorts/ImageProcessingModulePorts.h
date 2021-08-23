/*
 * ImageProcessingModulePorts.h
 *
 *  Created on: 2021Äê8ÔÂ22ÈÕ
 *      Author: h13
 */

#ifndef PLATFORM_USART_IMAGEPROCESSINGMODULEPORTS_IMAGEPROCESSINGMODULEPORTS_H_
#define PLATFORM_USART_IMAGEPROCESSINGMODULEPORTS_IMAGEPROCESSINGMODULEPORTS_H_

#include "RobotConfigs.h"
#include <stdint.h>

/**
 * @brief: Send data to image processing module through serialPort
 * @param: data
 */
void ImageProcessingModuleSerialSend(uint8_t data);

#endif /* PLATFORM_USART_IMAGEPROCESSINGMODULEPORTS_IMAGEPROCESSINGMODULEPORTS_H_ */
