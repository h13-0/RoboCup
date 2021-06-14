#include "JustFloat.h"

static USART_TypeDef *target = USART1;

void SetJustFloatSerialPort(USART_TypeDef *USARTx) {
	target = USARTx;
}

void SendJustFloatFrame(float data[], uint8_t len) {
	uint8_t *hexPtr = (uint8_t*) data;
	for (uint8_t index = 0; index < len * 4; index++) {
		while (LL_USART_IsActiveFlag_TC(target) == RESET)
			;
		LL_USART_TransmitData8(target, *hexPtr);
		hexPtr++;
	}

	while (LL_USART_IsActiveFlag_TC(target) == RESET)
		;
	LL_USART_TransmitData8(target, 0x00);

	while (LL_USART_IsActiveFlag_TC(target) == RESET)
		;
	LL_USART_TransmitData8(target, 0x00);

	while (LL_USART_IsActiveFlag_TC(target) == RESET)
		;
	LL_USART_TransmitData8(target, 0x80);

	while (LL_USART_IsActiveFlag_TC(target) == RESET)
		;
	LL_USART_TransmitData8(target, 0x7f);
}
