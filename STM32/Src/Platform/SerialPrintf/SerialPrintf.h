#ifndef _SERIALPRINTF_H_
#define _SERIALPRINTF_H_

#include <stdio.h>

#ifdef STM32F407xx
#include "stm32f4xx_ll_usart.h"
#endif

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_usart.h"
#endif

void SetSerialPrintfTarget(USART_TypeDef *USARTx);

#endif
