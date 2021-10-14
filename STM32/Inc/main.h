/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_Input____DF_Player_Mini_Busy_Pin LL_GPIO_PIN_2
#define GPIO_Input____DF_Player_Mini_Busy_GPIO_Port GPIOC
#define GPIO_Output____HC05_EN_Pin LL_GPIO_PIN_3
#define GPIO_Output____HC05_EN_GPIO_Port GPIOC
#define TIM2_CH1____RightEncoder_CHA_Pin LL_GPIO_PIN_0
#define TIM2_CH1____RightEncoder_CHA_GPIO_Port GPIOA
#define TIM2_CH2____RightEncoder_CHB_Pin LL_GPIO_PIN_1
#define TIM2_CH2____RightEncoder_CHB_GPIO_Port GPIOA
#define TIM5_CH3____Servo4_Pin LL_GPIO_PIN_2
#define TIM5_CH3____Servo4_GPIO_Port GPIOA
#define GPIO_EXIT4____Z_AxisLimitSensor_Pin LL_GPIO_PIN_4
#define GPIO_EXIT4____Z_AxisLimitSensor_GPIO_Port GPIOA
#define TIM3_CH1____Motor_D0_Pin LL_GPIO_PIN_6
#define TIM3_CH1____Motor_D0_GPIO_Port GPIOA
#define TIM3_CH2____Motor_D1_Pin LL_GPIO_PIN_7
#define TIM3_CH2____Motor_D1_GPIO_Port GPIOA
#define GPIO_Output____SSD1283_CS_Pin LL_GPIO_PIN_4
#define GPIO_Output____SSD1283_CS_GPIO_Port GPIOC
#define GPIO_Output____SSD1283_DC_Pin LL_GPIO_PIN_5
#define GPIO_Output____SSD1283_DC_GPIO_Port GPIOC
#define TIM3_CH3____Motor_D2_Pin LL_GPIO_PIN_0
#define TIM3_CH3____Motor_D2_GPIO_Port GPIOB
#define TIM3_CH4____Motor_D3_Pin LL_GPIO_PIN_1
#define TIM3_CH4____Motor_D3_GPIO_Port GPIOB
#define GPIO_Output____SSD1283_RST_Pin LL_GPIO_PIN_2
#define GPIO_Output____SSD1283_RST_GPIO_Port GPIOB
#define GPIO_Output____Z_AxisStepperSTEP_Pin LL_GPIO_PIN_12
#define GPIO_Output____Z_AxisStepperSTEP_GPIO_Port GPIOB
#define GPIO_Output____Z_AxisStepperDIR_Pin LL_GPIO_PIN_13
#define GPIO_Output____Z_AxisStepperDIR_GPIO_Port GPIOB
#define GPIO_Output____AL_AxisStepperSTEP_Pin LL_GPIO_PIN_14
#define GPIO_Output____AL_AxisStepperSTEP_GPIO_Port GPIOB
#define GPIO_Output____AL_AxisStepperDIR_Pin LL_GPIO_PIN_15
#define GPIO_Output____AL_AxisStepperDIR_GPIO_Port GPIOB
#define TIM8_CH1____LeftEncoder_CHA_Pin LL_GPIO_PIN_6
#define TIM8_CH1____LeftEncoder_CHA_GPIO_Port GPIOC
#define TIM8_CH2____LeftEncoder_CHB_Pin LL_GPIO_PIN_7
#define TIM8_CH2____LeftEncoder_CHB_GPIO_Port GPIOC
#define TIM1_CH1____Servo0_Pin LL_GPIO_PIN_8
#define TIM1_CH1____Servo0_GPIO_Port GPIOA
#define TIM1_CH2____Servo1_Pin LL_GPIO_PIN_9
#define TIM1_CH2____Servo1_GPIO_Port GPIOA
#define TIM1_CH3____Servo2_Pin LL_GPIO_PIN_10
#define TIM1_CH3____Servo2_GPIO_Port GPIOA
#define TIM1_CH4____Servo3_Pin LL_GPIO_PIN_11
#define TIM1_CH4____Servo3_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
