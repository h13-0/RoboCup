/*
 * SPI.c
 *
 *  Created on: 2021年6月5日
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

/**
 * @ps:    这部分操作我还是用中文解释吧... 虽然没什么特别的, 只是用宏定义来更优雅的完成超时处理.
 * @group: 超时检测相关宏
 *
 * @usage:
 * 		CheckTimeOutInit();
 * 		CheckTimeOut() while (LL_SPI_IsActiveFlag_TXE(target) == SET) TimeLimit()
 * 		CheckTimeOut() while (...) TimeLimit()
 *
 * @param: errorTimeLimit 最大等待次数
 * @parma: returnCode 触发超时后，函数返回值。
 *
 * @note:
 * 		CheckTimeOutInit();
 * 		CheckTimeOut() while (LL_SPI_IsActiveFlag_TXE(target) == SET) TimeLimit()
 *
 * 		最终等价于:
 *
 * 		uint16_t errorTimes = 0;
 * 		errorTimes = 0;   //拆成两句是为了防止连续使用时的多重定义问题.
 * 		while (LL_SPI_IsActiveFlag_TXE(target) == SET)
 * 		{
 * 			errorTimes ++;
 * 			if(errorTimes > errorTimeLimit)
 * 			{
 * 				return;
 * 			}
 * 		}
 */
#define CheckTimeOutInit() uint16_t errorTimes = 0;
#define CheckTimeOut()     errorTimes = 0;
#define TimeLimit(errorTimeLimit) {errorTimes ++; if(errorTimes > errorTimeLimit) return;}

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
inline __attribute__((always_inline)) void SPIWriteByte(uint8_t data) {

	CheckTimeOutInit();

	CheckTimeOut() while (LL_SPI_IsActiveFlag_BSY(target) == SET) TimeLimit(1000)
	CheckTimeOut() while (LL_SPI_IsActiveFlag_TXE(target) == RESET) TimeLimit(1000)

	LL_SPI_TransmitData8(target, data);
}

/**
 * @group: Set and Reset Pins of LCD.
 */
inline __attribute__((always_inline)) void LCDSetRST(void)
{
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
}

inline __attribute__((always_inline)) void LCDResetRST(void)
{
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
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
