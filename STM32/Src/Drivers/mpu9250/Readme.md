# 宏定义
MPL_LOG_NDEBUG=1,EMPL,MPU9250,EMPL_TARGET_STM32F4
# 链接libmpllib.lib
默认为M4, 其他lib见压缩包
Properties -> C/C++ Build -> Tool Settings -> MCU GCC Linker -> Miscellaneous -> Other flages -> add
```
"${workspace_loc:/${ProjName}/lib路径}"
```
eg:
```
"${workspace_loc:/${ProjName}/Src/mpu9250/dmp/mpl/liblibmplmpu.a}"
```

# demo
```
/*
 * App.c
 *
 *  Created on: 2021年5月11日
 *      Author: h13
 */

#include "Application.h"

#include "SerialPrintf.h"

#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


int App(USART_TypeDef *debugUart, I2C_HandleTypeDef *hi2c1)
{
	SetSerialPrintfTarget(debugUart);

	printf("MPU9250 Test\r\n");

	uint8_t ret = 0;

	ret = MPU9250_Init(hi2c1);

	while(ret)
	{
		printf("MPU9250 init failed with ret: %d\r\n", ret);
		HAL_Delay(500);
		ret = MPU9250_Init(hi2c1);
	}

	ret = mpu_dmp_init();

	while(ret)
	{
		printf("mpu_dmp init failed with ret: %d\r\n", ret);
		HAL_Delay(500);
		ret = mpu_dmp_init();
	}

	while(1);
}

```