/*
 * ImageProcessingModulePorts.c
 *
 *  Created on: 2021Äê8ÔÂ22ÈÕ
 *      Author: h13
 */
#include "ImageProcessingModulePorts.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

/**
 * @brief: Send data to image processing module through serialPort
 * @param: data
 */
__attribute__((always_inline)) inline void ImageProcessingModuleSerialSend(uint8_t data)
{
	while (LL_USART_IsActiveFlag_TC(ImageProcessingModulePorts) == RESET);
	LL_USART_TransmitData8(ImageProcessingModulePorts, data);
}
