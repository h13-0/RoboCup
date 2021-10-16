/*
 * SPI.c
 *
 *  Created on: 2021Äê6ÔÂ5ÈÕ
 *      Author: h13
 */

#include "SPI.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_gpio.h"
#endif

/**
 * @param: The SPI Port you selected.
 */
static SPI_TypeDef *target = SPI1;

#define errorTimeLimit 1000
#define wait(logicalExpression, returnCode) { uint16_t errorTimes = 0; while(logicalExpression){ errorTimes ++; if(errorTimes > errorTimeLimit){ return returnCode; }}}

/**
 * @brief: Initialize the SPI interface.
 * @note:  If multiple devices are mounted under the SPI interface, be aware of SPI initialization sequence and repeated initialization issues.
 */
void SPIInit(void) {
	LL_SPI_Enable(target);
}

/**
 * @brief: SPI write 8-bit data.
 * @param: data.
 */
inline __attribute__((always_inline)) inline SPI_Error_t SPIWriteByte(uint8_t data)
{
	wait(LL_SPI_IsActiveFlag_BSY(target) == SET, SPI_Busy);
	wait(LL_SPI_IsActiveFlag_TXE(target) == RESET, SPI_TransmissionTimeout);

	LL_SPI_TransmitData8(target, data);
	return SPI_OK;
}

/**
 * @group: Set and Reset Pins of LCD.
 */
inline __attribute__((always_inline)) void LCDSetRST(void)
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2);
}

inline __attribute__((always_inline)) void LCDResetRST(void)
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2);
}

inline __attribute__((always_inline)) void LCDSetCS(void)
{
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_4);
}

inline __attribute__((always_inline)) void LCDResetCS(void)
{
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_4);
}

inline __attribute__((always_inline)) void LCDSetDC(void)
{
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_5);
}

inline __attribute__((always_inline)) void LCDResetDC(void)
{
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_5);
}
