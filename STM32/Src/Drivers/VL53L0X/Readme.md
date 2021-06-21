# 正常使用
```
/*
 * App.c
 *
 *  Created on: 2021年6月19日
 *      Author: h13
 */

#include "SerialPrintf.h"
#include "vl53l0x_easy_api.h"

#include "stm32f1xx_ll_gpio.h"

int App(I2C_HandleTypeDef *i2cPort)
{
	SetSerialPrintfTarget(USART1);

	VL53L0X_Dev_t dev;

	VL53L0X_Error error = VL53L0X_ERROR_NONE;
	error = VL53l0X_Init(i2cPort, &dev, GPIOB, LL_GPIO_PIN_5);

	while(error != VL53L0X_ERROR_NONE)
	{
		printf("Error: %d\r\n", error);
		HAL_Delay(100);
	}

	while(1)
	{
		uint16_t distance = 0;
		VL53l0X_Ranging(&dev, &distance);
		printf("Distace: %d\r\n", distance);
	}

	return 0;
}
```

# 多设备使用
**必须**在Init每个新设备之后把该设备的地址移动到新的地址上, 后续才能继续使用。
```
/*
 * App.c
 *
 *  Created on: 2021年6月19日
 *      Author: h13
 */

#include "SerialPrintf.h"
#include "vl53l0x_easy_api.h"

#include "stm32f1xx_ll_gpio.h"

int App(I2C_HandleTypeDef *i2cPort)
{
	SetSerialPrintfTarget(USART1);

	VL53L0X_Dev_t dev;

	VL53L0X_Error error = VL53L0X_ERROR_NONE;
	error = VL53l0X_Init(i2cPort, &dev, GPIOB, LL_GPIO_PIN_5);

	while(error != VL53L0X_ERROR_NONE)
	{
		printf("Error: %d\r\n", error);
		HAL_Delay(100);
	}

	uint8_t ret = VL53l0X_SetAddr(&dev, 0x55);
	while(ret != VL53L0X_ERROR_NONE)
	{
		printf("Error: %d\r\n", ret);
		HAL_Delay(100);
	}

	while(1)
	{
		uint16_t distance = 0;
		VL53l0X_Ranging(&dev, &distance);
		printf("Distace: %d\r\n", distance);
	}

	return 0;
}
```
# 校准
```
/*
 * App.c
 *
 *  Created on: 2021年6月19日
 *      Author: h13
 */

#include "SerialPrintf.h"
#include "vl53l0x_easy_api.h"

#include "stm32f1xx_ll_gpio.h"

int App(I2C_HandleTypeDef *i2cPort)
{
	SetSerialPrintfTarget(USART1);

	VL53L0X_Dev_t dev;

	VL53L0X_Error error = VL53L0X_ERROR_NONE;
	error = VL53l0X_Init(i2cPort, &dev, GPIOB, LL_GPIO_PIN_5);

	while(error != VL53L0X_ERROR_NONE)
	{
		printf("Error: %d\r\n", error);
		HAL_Delay(100);
	}

	VL53l0X_Adjust(&dev);
}
```

然后把printf输出的对应部分复制到 `vl53l0x_easy_api.c` 的 `static void vl53l0x_adjust_init(void)` 中。
